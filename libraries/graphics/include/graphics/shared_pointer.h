/**
 * galactic-fall
 * Copyright (C) 2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__SHARED_POINTER_H
#define GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__SHARED_POINTER_H

#include <concepts>
#include <cstddef>
#include <type_traits>
#include <utility>

template<typename ValueType>
concept SharedObject = (std::is_object_v<ValueType> == true) && (std::is_array_v<ValueType> == false);

template<typename ValueType>
class SharedPointer;

template<SharedObject ValueType, typename ... ArgumentTypes>
[[nodiscard]]
auto MakeSharedPointer(ArgumentTypes && ... Arguments) -> SharedPointer<ValueType>;

namespace Detail
{
    struct ReferenceControlBlock
    {
        std::size_t m_ReferenceCount = 1;
        
        virtual ~ReferenceControlBlock() = default;
    };
    
    template<typename ValueType>
    struct ObjectControlBlock final : ReferenceControlBlock
    {
        template<typename... ArgumentTypes>
        explicit ObjectControlBlock(ArgumentTypes && ... Arguments) :
            m_Object{std::forward<ArgumentTypes>(Arguments) ...}
        {
        }
        
        ValueType m_Object;
    };
}

template<typename ValueType>
class SharedPointer
{
    static_assert(SharedObject<ValueType>);
    //~ static_assert(std::is_object_v<ValueType>);
    //~ static_assert(!std::is_array_v<ValueType>);
    
    template<typename>
    friend class SharedPointer;
    
    template<SharedObject MakeValueType, typename ... ArgumentTypes>
    friend auto MakeSharedPointer(ArgumentTypes && ... Arguments) -> SharedPointer<MakeValueType>;
    
public:
    constexpr SharedPointer() noexcept = default;
    
    constexpr SharedPointer(std::nullptr_t) noexcept
    {
    }
    
    SharedPointer(SharedPointer const & Other) noexcept :
        m_Pointer{Other.m_Pointer},
        m_ControlBlock{Other.m_ControlBlock}
    {
        m_AddReference();
    }
    
    template<typename OtherType>
        requires std::convertible_to<OtherType *, ValueType *>
    SharedPointer(SharedPointer<OtherType> const & Other) noexcept :
        m_Pointer{Other.m_Pointer},
        m_ControlBlock{Other.m_ControlBlock}
    {
        m_AddReference();
    }
    
    SharedPointer(SharedPointer && Other) noexcept :
        m_Pointer{std::exchange(Other.m_Pointer, nullptr)},
        m_ControlBlock{std::exchange(Other.m_ControlBlock, nullptr)}
    {
    }
    
    template<typename OtherType>
        requires std::convertible_to<OtherType *, ValueType *>
    SharedPointer(SharedPointer<OtherType> && Other) noexcept :
        m_Pointer{std::exchange(Other.m_Pointer, nullptr)},
        m_ControlBlock{std::exchange(Other.m_ControlBlock, nullptr)}
    {
    }
    
    ~SharedPointer()
    {
        m_ReleaseReference();
    }
    
    auto operator=(SharedPointer Other) noexcept -> SharedPointer &
    {
        Swap(Other);
        
        return *this;
    }
    
    auto Reset() noexcept -> void
    {
        auto Empty = SharedPointer{};
        
        Swap(Empty);
    }
    
    auto Swap(SharedPointer & Other) noexcept -> void
    {
        std::swap(m_Pointer, Other.m_Pointer);
        std::swap(m_ControlBlock, Other.m_ControlBlock);
    }
    
    [[nodiscard]]
    auto Get() const noexcept -> ValueType *
    {
        return m_Pointer;
    }
    
    [[nodiscard]]
    auto UseCount() const noexcept -> std::size_t
    {
        return ((m_ControlBlock != nullptr) ? (m_ControlBlock->m_ReferenceCount) : (0));
    }
    
    [[nodiscard]]
    auto HasValue() const noexcept -> bool
    {
        return m_Pointer != nullptr;
    }
    
    auto operator*() const noexcept -> ValueType &
    {
        return *m_Pointer;
    }
    
    auto operator->() const noexcept -> ValueType *
    {
        return m_Pointer;
    }
    
    template<SharedObject TargetType>
        requires (
            requires(ValueType * Pointer)
            {
                dynamic_cast<TargetType *>(Pointer);
            }
        )
    [[nodiscard]]
    auto DynamicCast() const noexcept -> SharedPointer<TargetType>
    {
        auto Result = SharedPointer<TargetType>{};
        
        if(auto Pointer = dynamic_cast<TargetType *>(m_Pointer); Pointer != nullptr)
        {
            Result.m_Pointer = Pointer;
            Result.m_ControlBlock = m_ControlBlock;
            Result.AddReference();
        }
        
        return Result;
    }
    
    [[nodiscard]]
    friend auto operator<=>(SharedPointer const & Left, SharedPointer const & Right) noexcept -> std::strong_ordering
    {
        return std::compare_three_way{}(Left.m_Pointer, Right.m_Pointer);
    }
    
    [[nodiscard]]
    friend auto operator==(SharedPointer const & Left, SharedPointer const & Right) noexcept -> bool
    {
        return Left.m_Pointer == Right.m_Pointer;
    }
    
private:
    auto m_AddReference() noexcept -> void
    {
        if(m_ControlBlock != nullptr)
        {
            m_ControlBlock->m_ReferenceCount += 1;
        }
    }
    
    auto m_ReleaseReference() noexcept -> void
    {
        if(m_ControlBlock != nullptr)
        {
            m_ControlBlock->m_ReferenceCount -= 1;
            if(m_ControlBlock->m_ReferenceCount == 0)
            {
                delete m_ControlBlock;
            }
        }
    }
    
    ValueType * m_Pointer = nullptr;
    Detail::ReferenceControlBlock * m_ControlBlock = nullptr;
};

template<SharedObject ValueType, typename... ArgumentTypes>
[[nodiscard]]
auto MakeSharedPointer(ArgumentTypes && ... Arguments) -> SharedPointer<ValueType>
{
    auto ControlBlock = new Detail::ObjectControlBlock<ValueType>(std::forward<ArgumentTypes>(Arguments) ...);
    auto Result = SharedPointer<ValueType>{};
    
    Result.m_Pointer = std::addressof(ControlBlock->m_Object);
    Result.m_ControlBlock = ControlBlock;
    
    return Result;
}

template<typename ValueType>
struct std::hash<SharedPointer<ValueType>>
{
    [[nodiscard]]
    auto operator()(SharedPointer<ValueType> const & Pointer) const noexcept -> std::size_t
    {
        return std::hash<ValueType *>{}(Pointer.Get());
    }
};

#endif
