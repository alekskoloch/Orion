#version 120

uniform vec2 u_resolution;
uniform vec2 u_size;
uniform vec2 u_pos;
uniform vec2 u_mouse;
uniform float u_time;
uniform float u_winHeight;

uniform vec2 u_thumbPos;
uniform vec2 u_thumbSize;
uniform float u_normalizedScroll;

uniform float u_hover;
uniform float u_thumbHover;
uniform float u_dragging;

const vec3 C_LEFT = vec3( 1.0, 0.0, 0.85 );
const vec3 C_RIGHT = vec3( 0.0, 0.8, 1.0 );
const vec3 C_BG = vec3( 0.08, 0.08, 0.12 );
const vec3 C_TRACK = vec3( 0.15, 0.15, 0.20 );
const vec3 C_THUMB = vec3( 0.85, 0.85, 0.95 );

const float BASE_BRIGHTNESS = 0.12;
const float MOUSE_RADIUS = 180.0;
const float MOUSE_INTENSITY = 0.3;
const float PULSE_SPEED = 0.06;
const float PULSE_INTENSITY = 0.05;

const float BORDER_AA = 1.0;
const float GLOW_WIDTH = 8.0;
const float GLOW_INTENSITY = 0.2;

const float THUMB_GLOW = 10.0;
const float THUMB_GLOW_INTENSITY = 0.25;

float roundedRectDist( vec2 p, vec2 center, vec2 size, float radius )
{
    vec2 d = abs( p - center ) - size * 0.5 + radius;
    return length( max( d, 0.0 ) ) - radius + min( max( d.x, d.y ), 0.0 );
}

void main()
{
    vec2 screenPos = gl_FragCoord.xy;
    screenPos.y = u_winHeight - screenPos.y;

    vec2 p = screenPos - u_pos;

    float halfW = u_size.x * 0.5;
    float halfH = u_size.y * 0.5;

    // Track background (rounded rectangle)
    float trackRadius = 6.0;
    vec2 trackCenter = vec2( 0.0, halfH );
    float d_track = roundedRectDist( p, trackCenter, u_size, trackRadius );
    float maskTrack = 1.0 - smoothstep( 0.0, BORDER_AA, d_track );

    // Thumb (rounded rectangle)
    // u_thumbPos is the CENTER of the thumb in world coordinates
    // We need to convert to local coordinates relative to track
    vec2 thumbCenter = u_thumbPos - u_pos;
    float thumbRadius = 6.0;
    float d_thumb = roundedRectDist( p, thumbCenter, u_thumbSize, thumbRadius );
    float maskThumb = 1.0 - smoothstep( 0.0, 1.5, d_thumb );
    float maskThumbBorder = smoothstep( thumbRadius + 1.5, thumbRadius - 1.5, d_thumb );

    // Mouse hover glow on track
    float mouseDist = length( p - u_mouse );
    float mouseLight = smoothstep( MOUSE_RADIUS, 0.0, mouseDist );
    mouseLight = pow( mouseLight, 2.0 );

    float pulse = 0.5 + 0.5 * sin( u_time * PULSE_SPEED );
    float totalBrightness = BASE_BRIGHTNESS + ( mouseLight * MOUSE_INTENSITY ) + ( pulse * PULSE_INTENSITY );

    // Track glow
    float glowExtend = 30.0;
    vec3 glowColor = vec3( 0.0 );
    if ( d_track > 0.0 && d_track < glowExtend )
    {
        float glowFactor = exp( -d_track * ( 1.0 / GLOW_WIDTH ) ) * GLOW_INTENSITY;
        glowColor = mix( C_RIGHT, C_LEFT, u_normalizedScroll ) * glowFactor * totalBrightness;
    }

    // Thumb glow - REDUCED to prevent clipping
    float thumbGlowBase = exp( -abs( d_thumb ) * ( 1.0 / THUMB_GLOW ) );
    float hoverBright = u_thumbHover * 0.3;
    float dragBright = u_dragging * 0.4;
    float thumbGlowFactor = thumbGlowBase * THUMB_GLOW_INTENSITY * ( 1.0 + u_hover * 0.4 + u_dragging * 0.6 );
    vec3 thumbGlow = mix( C_RIGHT, C_LEFT, u_normalizedScroll ) * thumbGlowFactor;

    // Thumb border gradient
    vec3 thumbBorderCol = mix( C_THUMB, mix( C_RIGHT, C_LEFT, u_normalizedScroll ), maskThumbBorder );

    // Compose final color
    vec3 finalColor = vec3( 0.0 );
    float finalAlpha = 0.0;

    // Track base
    finalColor = mix( finalColor, C_TRACK, maskTrack );
    finalAlpha = max( finalAlpha, maskTrack );

    // Add glows
    finalColor += glowColor;
    finalAlpha = max( finalAlpha, length( glowColor ) );

    // Thumb - rendered on top
    vec3 thumbColor = C_THUMB + hoverBright + dragBright;
    finalColor = mix( finalColor, thumbColor, maskThumb );
    finalAlpha = max( finalAlpha, maskThumb );

    // Thumb border gradient
    finalColor = mix( finalColor, thumbBorderCol, maskThumbBorder );
    finalAlpha = max( finalAlpha, maskThumbBorder );

    // Add thumb glow
    finalColor += thumbGlow;
    finalAlpha = max( finalAlpha, length( thumbGlow ) );

    gl_FragColor = vec4( finalColor, clamp( finalAlpha, 0.0, 1.0 ) );
}
