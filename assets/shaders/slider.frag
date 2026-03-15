#version 120

uniform vec2 u_resolution;
uniform vec2 u_size;
uniform vec2 u_pos;
uniform vec2 u_mouse;
uniform float u_time;
uniform float u_winHeight;

uniform float u_value;        // 0.0 - 1.0
uniform float u_hover;        // 0.0 or 1.0
uniform float u_dragging;     // 0.0 or 1.0
uniform float u_releaseTime;  // time since last release

const vec3 C_LEFT = vec3( 1.0, 0.0, 0.85 );
const vec3 C_RIGHT = vec3( 0.0, 0.8, 1.0 );
const vec3 C_BG = vec3( 0.05, 0.05, 0.08 );
const vec3 C_TRACK = vec3( 0.12, 0.12, 0.16 );
const vec3 C_HANDLE = vec3( 0.95, 0.95, 1.0 );

const float BASE_BRIGHTNESS = 0.08;
const float MOUSE_RADIUS = 220.0;
const float MOUSE_INTENSITY = 0.25;
const float PULSE_SPEED = 0.08;
const float PULSE_INTENSITY = 0.04;

const float BORDER_AA = 1.0;
const float GLOW_WIDTH = 4.0;
const float GLOW_INTENSITY = 0.15;

const float HANDLE_RADIUS = 14.0;
const float HANDLE_GLOW = 12.0;
const float HANDLE_GLOW_INTENSITY = 0.2;

// Release sweep effect - FIXED SPEED
const float SWEEP_SPEED = 2000.0; // pixels per second
const float SWEEP_BRIGHTNESS = 3.0;
const float SWEEP_WIDTH = 30.0;

float circleDist( vec2 p, vec2 center, float radius )
{
    return length( p - center ) - radius;
}

void main()
{
    vec2 screenPos = gl_FragCoord.xy;
    screenPos.y = u_winHeight - screenPos.y;

    vec2 p = screenPos - u_pos;

    float halfH = u_size.y * 0.5;
    float halfW = u_size.x * 0.5;

    // Background canvas
    float glowExtend = 50.0;
    float canvasW = halfW + glowExtend;
    float canvasH = halfH + glowExtend;
    float d_canvas = max( abs( p.x ) - canvasW, abs( p.y ) - canvasH );
    float maskCanvas = 1.0 - smoothstep( 0.0, BORDER_AA, d_canvas );

    // Track geometry (rounded rectangle)
    float trackWidth = u_size.x * 0.7;
    float trackHeight = 8.0;
    vec2 trackCenter = vec2( 0.0, 0.0 );
    vec2 trackP = abs( p - trackCenter ) - vec2( trackWidth * 0.5, trackHeight * 0.5 );
    float d_track = length( max( trackP, 0.0 ) ) + min( max( trackP.x, trackP.y ), 0.0 ) - trackHeight * 0.5;
    float maskTrack = 1.0 - smoothstep( 0.0, 1.5, d_track );

    // Handle position
    float handleX = -trackWidth * 0.5 + u_value * trackWidth;
    vec2 handleCenter = vec2( handleX, 0.0 );
    float d_handle = circleDist( p, handleCenter, HANDLE_RADIUS );
    float maskHandle = 1.0 - smoothstep( 0.0, 1.5, d_handle );
    float maskHandleBorder = smoothstep( HANDLE_RADIUS + 1.5, HANDLE_RADIUS - 1.5, d_handle );

    // FIXED gradient based on track position (not slider value)
    float trackT = ( p.x + trackWidth * 0.5 ) / trackWidth;
    vec3 gradientColor = mix( C_RIGHT, C_LEFT, clamp( trackT, 0.0, 1.0 ) );

    // Track fill area: from LEFT edge to HANDLE position
    float fillStartX = -trackWidth * 0.5;
    float fillEndX = handleX;
    float fillWidth = max( fillEndX - fillStartX, 1.0 );
    
    float fillT = ( p.x - fillStartX ) / fillWidth;
    fillT = clamp( fillT, 0.0, 1.0 );
    vec3 fillGradient = mix( C_RIGHT, C_LEFT, fillT );
    
    // Fill mask - from left edge to handle
    float isLeftOfHandle = step( p.x, handleX );
    float d_fill = length( max( abs( p - vec2( ( fillStartX + fillEndX ) * 0.5, 0.0 ) ) - vec2( fillWidth * 0.5, trackHeight * 0.5 ), 0.0 ) ) - trackHeight * 0.5;
    float maskFill = 1.0 - smoothstep( 0.0, 1.5, d_fill );
    maskFill *= isLeftOfHandle;

    // Mouse hover glow on canvas
    float mouseDist = length( p - u_mouse );
    float mouseLight = smoothstep( MOUSE_RADIUS, 0.0, mouseDist );
    mouseLight = pow( mouseLight, 2.0 );

    float pulse = 0.5 + 0.5 * sin( u_time * PULSE_SPEED );
    float totalBrightness = BASE_BRIGHTNESS + ( mouseLight * MOUSE_INTENSITY ) + ( pulse * PULSE_INTENSITY );

    // Glow effect on canvas border
    vec3 glowColor = vec3( 0.0 );
    if ( d_canvas > 0.0 && d_canvas < glowExtend )
    {
        float glowFactor = exp( -d_canvas * ( 1.0 / GLOW_WIDTH ) ) * GLOW_INTENSITY;
        glowColor = gradientColor * glowFactor * totalBrightness;
    }

    // Handle glow - REDUCED to prevent clipping
    float handleGlowBase = exp( -abs( d_handle ) * ( 1.0 / HANDLE_GLOW ) );
    float handleGlowFactor = handleGlowBase * HANDLE_GLOW_INTENSITY * ( 1.0 + u_hover * 0.5 + u_dragging * 0.8 );
    vec3 handleGlow = gradientColor * handleGlowFactor;

    // Hover brighten on handle
    float hoverBright = u_hover * 0.25;
    float dragBright = u_dragging * 0.35;

    // Release SWEEP effect - FIXED SPEED from LEFT edge TO HANDLE position
    float timeSinceRelease = u_time - u_releaseTime;
    
    // Sweep travels at fixed speed (pixels/second)
    float sweepDistance = handleX - (-trackWidth * 0.5 );
    float sweepDuration = sweepDistance / SWEEP_SPEED;
    float sweepProgress = timeSinceRelease / max( sweepDuration, 0.001 );
    
    float inSweepWindow = step( 0.0, sweepProgress ) * ( 1.0 - step( 1.0, sweepProgress ) );
    
    // Sweep starts at left edge and travels to handle position
    float sweepStartX = -trackWidth * 0.5;
    float sweepEndX = handleX;
    float sweepCurrentX = mix( sweepStartX, sweepEndX, sweepProgress );
    
    // Sweep is a vertical band that moves from left to handle
    float distToSweep = abs( p.x - sweepCurrentX );
    float sweepMask = exp( -distToSweep * distToSweep / ( SWEEP_WIDTH * SWEEP_WIDTH ) ) * SWEEP_BRIGHTNESS * inSweepWindow;
    
    // Sweep only visible on track AND only left of handle
    sweepMask *= maskTrack * isLeftOfHandle;
    
    // Sweep color is bright white
    vec3 sweepColor = vec3( 1.0, 1.0, 1.0 ) * sweepMask;

    // Compose final color - HANDLE MUST BE LAST to render on top
    // NO BACKGROUND - transparent so tab hex effect shows through
    vec3 finalColor = vec3( 0.0 );
    float finalAlpha = 0.0;

    // Track base (only where track is)
    finalColor = mix( finalColor, C_TRACK, maskTrack );
    finalAlpha = max( finalAlpha, maskTrack );

    // Track fill with gradient (left portion up to handle)
    vec3 fillColor = mix( finalColor, fillGradient, maskFill );
    finalColor = mix( finalColor, fillColor, maskFill * maskTrack );
    finalAlpha = max( finalAlpha, maskFill * maskTrack );

    // Add release sweep (on top of fill, but UNDER handle)
    finalColor += sweepColor;
    finalAlpha = max( finalAlpha, sweepMask );

    // Handle - rendered LAST so it's always on top of everything
    vec3 handleColor = C_HANDLE + hoverBright + dragBright;
    finalColor = mix( finalColor, handleColor, maskHandle );
    finalAlpha = max( finalAlpha, maskHandle );
    
    // Handle border gradient
    vec3 handleBorderCol = mix( finalColor, gradientColor, maskHandleBorder );
    finalAlpha = max( finalAlpha, maskHandleBorder );
    finalColor = mix( finalColor, handleBorderCol, maskHandleBorder );

    // Add glows (only where they exist)
    finalColor += glowColor;
    finalAlpha = max( finalAlpha, length( glowColor ) );
    
    finalColor += handleGlow;
    finalAlpha = max( finalAlpha, length( handleGlow ) );

    // Track border accent
    float trackBorder = smoothstep( trackHeight * 0.5 + 1.5, trackHeight * 0.5 - 0.5, d_track );
    vec3 borderAccent = mix( finalColor, gradientColor, trackBorder * 0.4 );
    finalAlpha = max( finalAlpha, trackBorder * 0.4 );
    finalColor = mix( finalColor, borderAccent, trackBorder * 0.4 );

    gl_FragColor = vec4( finalColor, clamp( finalAlpha, 0.0, 1.0 ) );
}
