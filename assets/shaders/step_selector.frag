#version 120

uniform vec2 u_resolution;
uniform vec2 u_size;
uniform vec2 u_pos;
uniform vec2 u_mouse;
uniform float u_time;
uniform float u_winHeight;

uniform float u_value;        // 0.0 - 1.0 (normalized step position)
uniform float u_hover;        // 0.0 or 1.0
uniform float u_dragging;     // 0.0 or 1.0
uniform float u_releaseTime;  // time (in seconds) when release happened
uniform float u_stepCount;    // number of steps

// Colors from slider.frag
const vec3 C_LEFT = vec3( 1.0, 0.0, 0.85 );
const vec3 C_RIGHT = vec3( 0.0, 0.8, 1.0 );
const vec3 C_TRACK = vec3( 0.12, 0.12, 0.16 );

const float BORDER_AA = 1.0;

const float SEGMENT_GAP = 3.0;
const float SEGMENT_SKEW = 0.5;

// Distance to skewed segment (45-degree edges)
float segmentDist( vec2 p, float halfW, float halfH )
{
    float d_y = abs( p.y ) - halfH;

    vec2 normalLeft = normalize( vec2( -1.0, -SEGMENT_SKEW ) );
    float leftAnchor = -halfW + ( SEGMENT_SKEW * halfH );
    float d_left = dot( p - vec2( leftAnchor, 0.0 ), normalLeft );

    vec2 normalRight = normalize( vec2( 1.0, SEGMENT_SKEW ) );
    float rightAnchor = halfW - ( SEGMENT_SKEW * halfH );
    float d_right = dot( p - vec2( rightAnchor, 0.0 ), normalRight );

    return max( d_y, max( d_right, d_left ) );
}

void main()
{
    vec2 screenPos = gl_FragCoord.xy;
    screenPos.y = u_winHeight - screenPos.y;

    vec2 p = screenPos - u_pos;

    float halfH = u_size.y * 0.5;
    float halfW = u_size.x * 0.5;

    // Calculate segment positions
    float totalGapWidth = SEGMENT_GAP * ( u_stepCount - 1.0 );
    float segmentWidth = ( u_size.x - totalGapWidth ) / u_stepCount;
    float segmentHalfWidth = segmentWidth * 0.5;

    // Background - transparent
    vec3 finalColor = vec3( 0.0 );
    float finalAlpha = 0.0;

    // Calculate active step
    float currentStepFloat = u_value * ( u_stepCount - 1.0 );
    float nearestStep = round( currentStepFloat );

    // Time since release
    float timeSinceRelease = u_time - u_releaseTime;

    // Render each segment
    for ( int i = 0; i < 8; i++ )
    {
        if ( float( i ) < u_stepCount )
        {
            // Calculate segment boundaries
            float segmentStartX = -halfW + ( float( i ) * ( segmentWidth + SEGMENT_GAP ) );
            float segmentEndX = segmentStartX + segmentWidth;
            float segmentCenterX = ( segmentStartX + segmentEndX ) * 0.5;

            // Transform to local segment coordinates
            vec2 segP = p - vec2( segmentCenterX, 0.0 );

            // Distance to this segment (with 45-degree skew)
            float d = segmentDist( segP, segmentHalfWidth, halfH );

            // Segment mask
            float maskSegment = 1.0 - smoothstep( 0.0, BORDER_AA, d );

            // Check if this is the active segment
            float stepDiff = abs( float( i ) - nearestStep );
            float isCurrentStep = 1.0 - smoothstep( 0.3, 0.5, stepDiff );

            // EACH segment has its OWN gradient (left to right within segment)
            float segGradX = ( segP.x / segmentHalfWidth ) * 0.5 + 0.5;
            vec3 segmentGradient = mix( C_RIGHT, C_LEFT, clamp( segGradX, 0.0, 1.0 ) );

            // Segment color
            vec3 segmentColor = vec3( 0.0 );

            if ( isCurrentStep > 0.5 )
            {
                // ============ ACTIVE SEGMENT ============
                
                // Base: bright gradient
                segmentColor = segmentGradient;
                segmentColor *= 1.4;

                // --- HOVER EFFECT: Simple brighten (no burst) ---
                if ( u_hover > 0.5 )
                {
                    segmentColor += vec3( 0.35 );
                }

                // --- DRAGGING: Extra bright ---
                if ( u_dragging > 0.5 )
                {
                    segmentColor += vec3( 0.5 );
                }

                // --- RELEASE SWEEP: FAST burst from center outward ---
                if ( timeSinceRelease > 0.0 && timeSinceRelease < 0.25 )
                {
                    // Two bright bars expanding from center - VERY FAST
                    float sweepRadius = timeSinceRelease * 2500.0;  // Very fast: 2500 px/s
                    float distFromCenter = abs( segP.x );
                    float distToSweep = abs( distFromCenter - sweepRadius );
                    
                    // Bright, narrow sweep line
                    float sweepMask = exp( -distToSweep * distToSweep / 30.0 ) * 6.0;
                    
                    // Fade out over 0.25 seconds (fast!)
                    float sweepFade = 1.0 - smoothstep( 0.0, 0.25, timeSinceRelease );
                    sweepMask *= sweepFade;
                    
                    // Only on this segment
                    sweepMask *= maskSegment;
                    
                    // Add bright white sweep
                    finalColor += vec3( 1.0, 1.0, 1.0 ) * sweepMask;
                }
            }
            else
            {
                // ============ INACTIVE SEGMENT ============
                // Dark with subtle gradient - NO HOVER EFFECT
                segmentColor = segmentGradient * 0.12;
            }

            // Blend segment
            finalColor = mix( finalColor, segmentColor, maskSegment );
            finalAlpha = max( finalAlpha, maskSegment );
        }
    }

    gl_FragColor = vec4( finalColor, clamp( finalAlpha, 0.0, 1.0 ) );
}
