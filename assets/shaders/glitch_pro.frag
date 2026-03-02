#version 120

uniform sampler2D texture;
uniform float u_time;
uniform float u_intensity;

const float SPEED_BLOCK_UPDATE = 30.0;
const float SPEED_GLOBAL_NOISE = 20.0;

const float BLOCK_MIN_DENSITY = 10.0;
const float BLOCK_MAX_DENSITY = 100.0;
const float BLOCK_THRESH_STABILITY = 0.8;

const float SHIFT_AMPLITUDE = 0.06;
const float CHROMATIC_ABR = 0.02;
const float GRAIN_INTENSITY = 0.005;

const float OPACITY_MIN = 0.4;
const float OPACITY_MAX = 1.0;
const float BRIGHTNESS_BOOST = 2.0;
const float CENTER_WHITE_BOOST = 0.5;

const float SCANLINE_DENSITY = 1200.0;
const float SCANLINE_OPACITY = 0.15;

float random( vec2 st ) { return fract( sin( dot( st.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453 ); }

float noise( float p )
{
    float fl = floor( p );
    float fc = fract( p );
    return mix( random( vec2( fl ) ), random( vec2( fl + 1.0 ) ), fc );
}

float quantize( float value, float levels ) { return floor( value * levels ) / levels; }

float modifyChannel( float alpha, float opacity, float boost ) { return alpha * opacity * boost; }

void main()
{
    vec2 uv = gl_TexCoord[ 0 ].xy;

    if ( u_intensity <= 0.01 )
    {
        gl_FragColor = texture2D( texture, uv ) * gl_Color;
        return;
    }

    float timeStep = floor( u_time * SPEED_BLOCK_UPDATE );
    float globalNoise = noise( u_time * SPEED_GLOBAL_NOISE );

    float realIntensity = u_intensity * ( 0.4 + 0.6 * globalNoise );

    float maxShift = SHIFT_AMPLITUDE * realIntensity;
    float chromaticAbr = CHROMATIC_ABR * realIntensity;

    float currentDensity = mix( BLOCK_MIN_DENSITY, BLOCK_MAX_DENSITY, random( vec2( timeStep, 7.0 ) ) );

    float blockY = quantize( uv.y, currentDensity );

    float blockShift = random( vec2( blockY, timeStep ) );
    blockShift = ( blockShift - 0.5 ) * 2.0;

    float stability = 1.0 - realIntensity;
    if ( abs( blockShift ) < stability * BLOCK_THRESH_STABILITY )
    {
        blockShift = 0.0;
    }

    float blockOpacity = mix( OPACITY_MIN, OPACITY_MAX, random( vec2( blockY, timeStep * 2.0 ) ) );
    float brightnessBoost = 1.0 + abs( blockShift ) * BRIGHTNESS_BOOST;

    float baseShiftX = blockShift * maxShift;
    float grain = ( random( uv + timeStep ) - 0.5 ) * GRAIN_INTENSITY * realIntensity;

    vec2 rUV = uv + vec2( baseShiftX + chromaticAbr + grain, 0.0 );
    vec2 gUV = uv + vec2( baseShiftX - chromaticAbr + grain, 0.0 );

    vec2 bUV = uv + vec2( baseShiftX - chromaticAbr * 0.5, 0.0 );

    rUV = clamp( rUV, 0.0, 1.0 );
    gUV = clamp( gUV, 0.0, 1.0 );
    bUV = clamp( bUV, 0.0, 1.0 );

    float r = texture2D( texture, rUV ).a;
    float g = texture2D( texture, gUV ).a;
    float b = texture2D( texture, bUV ).a;

    r = modifyChannel( r, blockOpacity, brightnessBoost );
    g = modifyChannel( g, blockOpacity, brightnessBoost );
    b = modifyChannel( b, blockOpacity, brightnessBoost );

    vec3 color = vec3( r, g, b );

    float overlap = min( r, min( g, b ) );
    color += vec3( overlap * CENTER_WHITE_BOOST );

    float finalAlpha = max( max( r, g ), b );

    if ( finalAlpha > 0.05 )
    {
        float scanLine = sin( uv.y * SCANLINE_DENSITY ) * SCANLINE_OPACITY;
        color -= scanLine;
    }

    gl_FragColor = vec4( color, finalAlpha ) * gl_Color;
}