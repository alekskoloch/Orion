#pragma once

namespace utils
{

namespace FormatValue
{
enum class Precision : std::int8_t
{
    ToThousands = -3,
    ToHundreds = -2,
    ToTens = -1,
    ToOnes = 0,
    ToOneDecimal = 1,
    ToTwoDecimals = 2,
    ToThreeDecimals = 3,
    ToFourDecimals = 4
};
} // namespace FormatValue

template < FormatValue::Precision P = FormatValue::Precision::ToOnes, typename ValueType >
    requires std::is_arithmetic_v< ValueType >
auto formatValue( ValueType value ) -> std::string
{
    constexpr double base = 10.0;

    constexpr int p_val = static_cast< int >( P );
    double factor = std::pow( base, -p_val );

    double rounded = std::round( static_cast< double >( value ) / factor ) * factor;

    if constexpr ( p_val > 0 )
    {
        return std::format( "{:.{}f}", rounded, p_val );
    }

    return std::format( "{:.0f}", rounded );
}

} // namespace utils