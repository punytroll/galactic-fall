/**
 * galactic-fall
 * Copyright (C) 2008-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__VISUALIZATION_PROTOTYPE_H
#define GALACTIC_FALL__SOURCE__VISUALIZATION_PROTOTYPE_H

#include <map>
#include <optional>
#include <string>

#include <algebra/vector3f.h>

namespace Graphics
{
	class Model;
	class Style;
}

class VisualizationPrototype
{
public:
	// constructor & destructor
	VisualizationPrototype(void);
	VisualizationPrototype(const VisualizationPrototype * VisualizationPrototype);
	VisualizationPrototype(const VisualizationPrototype & VisualizationPrototype);
	~VisualizationPrototype(void);
	// getters
	const Graphics::Model * GetModel(void) const;
	std::map< std::string, Graphics::Style * > & GetPartStyles(void);
	const std::map< std::string, Graphics::Style * > & GetPartStyles(void) const;
	auto GetMarkerLength(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<float> const &;
	auto GetMarkerOrientation(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<Quaternion> const &;
	auto GetMarkerPosition(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<Vector3f> const &;
	// setters
	void SetModel(const Graphics::Model * Model);
	/**
	 * @note Passes memory management responsibility for @a PartStyle to the visualization prototype.
	 **/
	void SetPartStyle(const std::string & Identifier, Graphics::Style * PartStyle);
private:
	const Graphics::Model * _Model;
	std::map< std::string, Graphics::Style * > _PartStyles;
};

inline const Graphics::Model * VisualizationPrototype::GetModel(void) const
{
	return _Model;
}

inline std::map< std::string, Graphics::Style * > & VisualizationPrototype::GetPartStyles(void)
{
	return _PartStyles;
}

inline const std::map< std::string, Graphics::Style * > & VisualizationPrototype::GetPartStyles(void) const
{
	return _PartStyles;
}

inline void VisualizationPrototype::SetModel(const Graphics::Model * Model)
{
	_Model = Model;
}

#endif
