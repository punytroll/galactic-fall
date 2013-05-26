#ifndef BOXES_H
#define BOXES_H
#include <iostream>
#include <deque>

namespace Boxes
{
 class Size
 {
 public:
  Size(void);
  Size(float fWidth, float fHeight);
  Size(const Boxes::Size & OtherSize);
  void vMaximize(const Boxes::Size & OtherSize);
  bool operator!=(const Boxes::Size & OtherSize) const;
  Boxes::Size operator*(float fScalar);
  Boxes::Size operator/(float fScalar);
  float m_fWidth;
  float m_fHeight;
 };

 std::ostream & operator<<(std::ostream & OStream, const Boxes::Size & Size);
}



namespace Boxes
{
 class Box;

 class Layout
 {
 public:
  enum Method
  {
   NONE
  };

  class AddOptions
  {
  public:
   AddOptions(int iMethod);
   virtual ~AddOptions(void);
   int m_iMethod;
  };

  class ChildOptions
  {
  public:
   ChildOptions(void);
   virtual ~ChildOptions(void);

   Boxes::Box * m_pBox;
   float m_fPositionX;
   float m_fPositionY;
  };

  class iterator
  {
  public:
   iterator(std::deque< Boxes::Layout::ChildOptions * > & Childs, std::deque< Boxes::Layout::ChildOptions * >::size_type stIndex) :
    m_stIndex(stIndex),
    m_Childs(Childs)
   {
   }

   Boxes::Box * operator*(void)
   {
    return m_Childs[m_stIndex]->m_pBox;
   }

   Boxes::Box * operator->(void)
   {
    return m_Childs[m_stIndex]->m_pBox;
   }

   Boxes::Layout::ChildOptions * pGetOptions(void)
   {
    return m_Childs[m_stIndex];
   }

   Boxes::Layout::iterator & operator++(void)
   {
    ++m_stIndex;

    return *this;
   }

   bool operator!=(const Boxes::Layout::iterator & Iterator)
   {
    return m_stIndex != Iterator.m_stIndex;
   }
  private:
   std::deque< Boxes::Layout::ChildOptions * >::size_type m_stIndex;
   std::deque< Boxes::Layout::ChildOptions * > & m_Childs;
  };

  virtual ~Layout(void);
  virtual void vAdd(Boxes::Box * pBox, const Boxes::Layout::AddOptions & Options) = 0;
  void vDelete(Boxes::Box * pBox);
  virtual bool bHandleResize(const Boxes::Size & BoxSize) = 0;
  Boxes::Size GetMinimumSize(void);
  Boxes::Layout::iterator begin(void);
  Boxes::Layout::iterator end(void);
 protected:
  std::deque< Boxes::Layout::ChildOptions * > m_Boxes;
  Size m_MinimumSize;
 };
}




namespace Boxes
{
 class Box
 {
 public:
  Box(void);
  virtual ~Box(void);
  Boxes::Size GetSize(void) const;
  Boxes::Size GetMinimumSize(void) const;
  void vSetMinimumSize(const Boxes::Size & MinimumSize);
  void vAddBox(Boxes::Box * pBox, const Boxes::Layout::AddOptions & Options);
  void vAddBox(Boxes::Box * pBox);
  void vDeleteBox(Boxes::Box * pBox);
  Boxes::Box * pGetParent(void);
  void vSetLayout(Boxes::Layout * pLayout);
  Boxes::Layout * pGetLayout(void);
 private:
  void vHandleResize(void);
  Boxes::Layout * m_pLayout;
  Boxes::Box * m_pParent;
  Boxes::Size m_MinimumSize;
 };

 std::ostream & operator<<(std::ostream & OStream, const Boxes::Box & Box);
}
namespace Boxes
{
 class Box;

 class FreeLayout : public Boxes::Layout
 {
 public:
  class AddOptions : public Boxes::Layout::AddOptions
  {
  public:
   AddOptions(float fX, float fY);
   float m_fX;
   float m_fY;
  };

  virtual void vAdd(Boxes::Box * pBox, const Boxes::Layout::AddOptions & AddOptions);
  virtual bool bHandleResize(const Boxes::Size & BoxSize);
 };
}
namespace Boxes
{
 class HorizontalLayout : public Boxes::Layout
 {
 public:
  enum Method
  {
   PACK_BEGIN,
   PACK_END
  };

  class ChildOptions : public Boxes::Layout::ChildOptions
  {
  public:
   ChildOptions(void);
   virtual ~ChildOptions(void);
   Boxes::HorizontalLayout::Method m_Method;
  };

  virtual void vAdd(Boxes::Box * pBox, const Boxes::Layout::AddOptions & Options);
  virtual bool bHandleResize(const Boxes::Size & BoxSize);
 };
}
namespace Boxes
{
 class Box;

 class VerticalLayout : public Boxes::Layout
 {
 public:
  enum Method
  {
   PACK_BEGIN,
   PACK_END
  };

  class ChildOptions : public Boxes::Layout::ChildOptions
  {
  public:
   ChildOptions(void);
   virtual ~ChildOptions(void);
   Boxes::VerticalLayout::Method m_Method;
  };

  virtual void vAdd(Boxes::Box * pBox, const Boxes::Layout::AddOptions & AddOptions);
  virtual bool bHandleResize(const Boxes::Size & BoxSize);
 };
}
#endif

