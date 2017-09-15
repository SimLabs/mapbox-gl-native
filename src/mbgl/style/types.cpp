#include <mbgl/style/types.hpp>
#include <mbgl/util/enum.hpp>

namespace mbgl {

using namespace style;

MBGL_ENUM_BEGIN(SourceType) {
    { SourceType::Vector, "vector" },
    { SourceType::Raster, "raster" },
    { SourceType::GeoJSON, "geojson" },
    { SourceType::Video, "video" },
    { SourceType::Annotations, "annotations" },
    { SourceType::Image, "image" },
} MBGL_ENUM_END(SourceType);

MBGL_ENUM_BEGIN(VisibilityType) {
    { VisibilityType::Visible, "visible" },
    { VisibilityType::None, "none" },
} MBGL_ENUM_END(VisibilityType);

MBGL_ENUM_BEGIN(TranslateAnchorType) {
    { TranslateAnchorType::Map, "map" },
    { TranslateAnchorType::Viewport, "viewport" },
} MBGL_ENUM_END(TranslateAnchorType);

MBGL_ENUM_BEGIN(RotateAnchorType) {
    { RotateAnchorType::Map, "map" },
    { RotateAnchorType::Viewport, "viewport" },
} MBGL_ENUM_END(RotateAnchorType);

MBGL_ENUM_BEGIN(CirclePitchScaleType) {
    { CirclePitchScaleType::Map, "map" },
    { CirclePitchScaleType::Viewport, "viewport" },
} MBGL_ENUM_END(CirclePitchScaleType);

MBGL_ENUM_BEGIN(LineCapType) {
    { LineCapType::Round, "round" },
    { LineCapType::Butt, "butt" },
    { LineCapType::Square, "square" },
} MBGL_ENUM_END(LineCapType);

MBGL_ENUM_BEGIN(LineJoinType) {
    { LineJoinType::Miter, "miter" },
    { LineJoinType::Bevel, "bevel" },
    { LineJoinType::Round, "round" },
    { LineJoinType::FakeRound, "fakeround" },
    { LineJoinType::FlipBevel, "flipbevel" },
} MBGL_ENUM_END(LineJoinType);

MBGL_ENUM_BEGIN(SymbolPlacementType) {
    { SymbolPlacementType::Point, "point" },
    { SymbolPlacementType::Line, "line" },
} MBGL_ENUM_END(SymbolPlacementType);

MBGL_ENUM_BEGIN(TextAnchorType) {
    { TextAnchorType::Center, "center" },
    { TextAnchorType::Left, "left" },
    { TextAnchorType::Right, "right" },
    { TextAnchorType::Top, "top" },
    { TextAnchorType::Bottom, "bottom" },
    { TextAnchorType::TopLeft, "top-left" },
    { TextAnchorType::TopRight, "top-right" },
    { TextAnchorType::BottomLeft, "bottom-left" },
    { TextAnchorType::BottomRight, "bottom-right" }
} MBGL_ENUM_END(TextAnchorType);

MBGL_ENUM_BEGIN(TextJustifyType) {
    { TextJustifyType::Center, "center" },
    { TextJustifyType::Left, "left" },
    { TextJustifyType::Right, "right" },
} MBGL_ENUM_END(TextJustifyType);

MBGL_ENUM_BEGIN(TextTransformType) {
    { TextTransformType::None, "none" },
    { TextTransformType::Uppercase, "uppercase" },
    { TextTransformType::Lowercase, "lowercase" },
} MBGL_ENUM_END(TextTransformType);

MBGL_ENUM_BEGIN(AlignmentType) {
    { AlignmentType::Map, "map" },
    { AlignmentType::Viewport, "viewport" },
    { AlignmentType::Auto, "auto" },
} MBGL_ENUM_END(AlignmentType);

MBGL_ENUM_BEGIN(IconTextFitType) {
    { IconTextFitType::None, "none" },
    { IconTextFitType::Both, "both" },
    { IconTextFitType::Width, "width" },
    { IconTextFitType::Height, "height" },
} MBGL_ENUM_END(IconTextFitType);

MBGL_ENUM_BEGIN(LightAnchorType) {
    { LightAnchorType::Map, "map" },
    { LightAnchorType::Viewport, "viewport" }
} MBGL_ENUM_END(LightAnchorType);

} // namespace mbgl
