#version 120

uniform vec2 u_resolution;
uniform vec2 u_size;
uniform vec2 u_pos;
uniform vec2 u_mouse;
uniform float u_time;
uniform float u_winHeight;
uniform float u_skew;

const vec3 C_HEX = vec3( 1.00, 0.0, 0.65 );
const vec3 C_LEFT = vec3( 1.0, 0.0, 0.7 );
const vec3 C_RIGHT = vec3( 0.05, 0.95, 1.0 );
const vec3 C_BG = vec3( 0.02, 0.02, 0.04 );

const float HEX_SCALE = 38.0;
const float HEX_LINE_WIDTH = 0.06;
const float HEX_LINE_SHARP = 0.04;

const float BASE_BRIGHTNESS = 0.05;
const float MOUSE_RADIUS = 220.0;
const float MOUSE_INTENSITY = 0.3;
const float PULSE_SPEED = 0.1;
const float PULSE_INTENSITY = 0.05;

const float VIGNETTE_WIDTH = 500.0;
const float VIGNETTE_OFFSET = 80.0;

const float BORDER_WIDTH = 1.5;
const float BORDER_AA = 1.5;
const float GLOW_WIDTH = 3.0;
const float GLOW_INTENSITY = 0.4;

float hexDist( vec2 p )
{
    p = abs( p );
    float c = dot( p, normalize( vec2( 1.0, 1.732 ) ) );
    return max( c, p.x );
}

float getHexGrid( vec2 p )
{
    vec2 r = vec2( 1.0, 1.732 );
    vec2 h = r * 0.5;
    vec2 a = mod( p, r ) - h;
    vec2 b = mod( p - h, r ) - h;
    vec2 gv = dot( a, a ) < dot( b, b ) ? a : b;
    return 0.5 - hexDist( gv );
}

void main()
{
    vec2 screenPos = gl_FragCoord.xy;
    screenPos.y = u_winHeight - screenPos.y;

    vec2 p = screenPos - u_pos;

    float halfH = u_size.y * 0.5;
    float halfW = u_size.x * 0.5;

    float d_y = abs( p.y ) - halfH;

    float d_right = p.x - halfW;

    vec2 normalLeft = normalize( vec2( -1.0, -u_skew ) );
    float leftAnchor = -halfW + ( u_skew * halfH );
    float d_left = dot( p - vec2( leftAnchor, 0.0 ), normalLeft );

    float d = max( d_y, max( d_right, d_left ) );

    float maskShape = 1.0 - smoothstep( 0.0, BORDER_AA, d );

    float maskBorder = smoothstep( -BORDER_WIDTH - 1.0, -BORDER_WIDTH, d );
    maskBorder *= maskShape;

    vec3 glowColor = vec3( 0.0 );
    if ( d > 0.0 )
    {
        float glowFactor = exp( -d * ( 1.0 / GLOW_WIDTH ) ) * GLOW_INTENSITY;

        float gradX = ( p.x / halfW ) * 0.5 + 0.5;
        vec3 gCol = mix( C_LEFT, C_RIGHT, clamp( gradX, 0.0, 1.0 ) );

        glowColor = gCol * glowFactor;
    }

    float distFromCenter = abs( p.x );
    float distFromEdge = halfW - distFromCenter;

    float globalHexMask = smoothstep( VIGNETTE_OFFSET, VIGNETTE_WIDTH, distFromEdge );

    vec2 hexP = p;

    hexP.x -= halfW;

    float hexVal = getHexGrid( hexP / HEX_SCALE );
    float hexLines = smoothstep( HEX_LINE_WIDTH, HEX_LINE_SHARP, hexVal );

    float mouseDist = length( p - u_mouse );
    float mouseLight = smoothstep( MOUSE_RADIUS, 0.0, mouseDist );
    mouseLight = pow( mouseLight, 2.0 );

    float pulse = 0.5 + 0.5 * sin( u_time * PULSE_SPEED );

    float totalBrightness = ( BASE_BRIGHTNESS + ( mouseLight * MOUSE_INTENSITY ) + ( pulse * PULSE_INTENSITY ) );
    totalBrightness *= globalHexMask;

    vec3 finalHex = C_HEX * hexLines * totalBrightness;
    finalHex *= maskShape;

    float gradX = ( p.x / halfW ) * 0.5 + 0.5;
    vec3 borderCol = mix( C_LEFT, C_RIGHT, clamp( gradX, 0.0, 1.0 ) );

    vec3 finalColor = C_BG * maskShape;
    finalColor += finalHex;
    finalColor = mix( finalColor, borderCol, maskBorder );
    finalColor += glowColor;

    float finalAlpha = maskShape + length( glowColor );

    gl_FragColor = vec4( finalColor, clamp( finalAlpha, 0.0, 1.0 ) );
}