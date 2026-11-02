/**
 * galactic-fall
 * Copyright (C) 2015-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__LIBRARIES__GRAPHICS__INCLUDE__GRAPHICS__STYLE_H
#define GALACTIC_FALL__LIBRARIES__GRAPHICS__INCLUDE__GRAPHICS__STYLE_H

#include <optional>
#include <string>

#include <graphics/color_rgbo.h>

namespace Graphics
{
	class Style
	{
	public:
        Style() = default;
		explicit Style(Graphics::Style const * Style);
        
		Style(Graphics::Style const & Style) = delete;
		auto operator=(Graphics::Style const & Style) -> Graphics::Style & = delete;
        
		// getters
		auto GetDiffuseColor() const -> Graphics::ColorRGBO const &;
		auto GetProgramIdentifier() const -> std::string const &;
		auto GetShininess() const -> float;
		auto GetSpecularColor() const -> Graphics::ColorRGBO const &;
		auto HasDiffuseColor() const -> bool;
		auto HasShininess() const -> bool;
		auto HasSpecularColor() const -> bool;
		// setters
		auto SetDiffuseColor(std::optional<Graphics::ColorRGBO> DiffuseColor) -> void;
		auto SetProgramIdentifier(std::string ProgramIdentifier) -> void;
		auto SetShininess(std::optional<float> Shininess) -> void;
		auto SetSpecularColor(std::optional<Graphics::ColorRGBO> SpecularColor) -> void;
	private:
		std::optional<Graphics::ColorRGBO> m_DiffuseColor;
		std::string m_ProgramIdentifier;
		std::optional<float> m_Shininess;
		std::optional<Graphics::ColorRGBO> m_SpecularColor;
	private:
	};
	
	inline auto Style::GetProgramIdentifier() const -> std::string const &
	{
		return m_ProgramIdentifier;
	}
	
	inline auto Style::HasDiffuseColor() const -> bool
	{
		return m_DiffuseColor.has_value();
	}
	
	inline auto Style::HasShininess() const -> bool
	{
		return m_Shininess.has_value();
	}
	
	inline auto Style::HasSpecularColor() const -> bool
	{
		return m_SpecularColor.has_value();
	}
    
    inline auto Style::SetDiffuseColor(std::optional<Graphics::ColorRGBO> DiffuseColor) -> void
    {
        m_DiffuseColor = std::move(DiffuseColor);
    }
	
	inline auto Style::SetProgramIdentifier(std::string ProgramIdentifier) -> void
	{
		m_ProgramIdentifier = std::move(ProgramIdentifier);
	}
    
    inline auto Style::SetShininess(std::optional<float> Shininess) -> void
    {
        m_Shininess = std::move(Shininess);
    }

    inline auto Style::SetSpecularColor(std::optional<Graphics::ColorRGBO> SpecularColor) -> void
    {
        m_SpecularColor = std::move(SpecularColor);
    }
}

#endif
