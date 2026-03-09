#version 120

uniform vec2 u_pos;
uniform vec2 u_size;
uniform float u_skew;
uniform float u_winHeight;
uniform float u_bodyPosX;
uniform float u_bodyWidth;
uniform bool u_active;
uniform bool u_hover;

uniform vec2 u_clickPos;
uniform float u_clickTime;

const float BORDER_WIDTH = 3.5;
const vec3 C_LEFT  = vec3( 1.0, 0.0, 0.85 );
const vec3 C_RIGHT = vec3( 0.0, 0.8, 1.0 );
const vec3 C_BG    = vec3( 0.05, 0.05, 0.08 );

float sdTrapezoid(vec2 p, float width, float height, float skew) {
    float t = clamp(p.y / height, 0.0, 1.0);
    float leftLimit = skew * (1.0 - t);
    float rightLimit = width - (skew * (1.0 - t));
    float dLeft = p.x - leftLimit;
    float dRight = rightLimit - p.x;
    float dTop = p.y;
    float dBot = height - p.y;
    return min(min(dLeft, dRight), min(dTop, dBot));
}

void main() {
    vec2 glPos = gl_FragCoord.xy;
    glPos.y = u_winHeight - glPos.y;

    float globalT = (glPos.x - u_bodyPosX) / u_bodyWidth;
    vec3 accentColor = mix(C_LEFT, C_RIGHT, clamp(globalT, 0.0, 1.0));

    vec2 p = glPos - u_pos;
    
    float t = clamp(p.y / u_size.y, 0.0, 1.0);
    float leftLimit = u_skew * (1.0 - t);
    float rightLimit = u_size.x - (u_skew * (1.0 - t));
    float dLeft = p.x - leftLimit;
    float dRight = rightLimit - p.x;
    float dTop = p.y;
    
    float dist;
    if (u_active) {
        dist = min(min(dLeft, dRight), dTop);
    } else {
        float dBot = u_size.y - p.y;
        dist = min(min(dLeft, dRight), min(dTop, dBot));
    }

    if (dist < -1.0) discard;

    float borderMask = smoothstep(BORDER_WIDTH + 0.8, BORDER_WIDTH - 0.8, dist);
    vec3 finalColor = C_BG;

    if (u_active) {
        finalColor = C_BG;
        finalColor += accentColor * 0.15 * smoothstep(u_size.y, 0.0, p.y);
        borderMask = max(borderMask, smoothstep(2.0, 0.0, p.y));
        // Usunieto bottomFade, aby przywrocic boczne linie
    } else {
        finalColor *= 0.6;
        if (u_hover) {
            borderMask += 0.3 * smoothstep(15.0, 0.0, dist);
            finalColor += vec3(0.1);
        }
    }

    finalColor = mix(finalColor, accentColor, clamp(borderMask, 0.0, 1.0));

    float distClick = distance(glPos, u_clickPos);
    float speed = 2500.0;
    float radius = u_clickTime * speed;
    float waveWidth = 150.0;
    
    float wave = smoothstep(radius, radius - waveWidth, distClick) 
               - smoothstep(radius - waveWidth * 0.2, radius - waveWidth * 1.2, distClick);
    wave = max(0.0, wave);
    
    float fade = exp(-u_clickTime * 4.0);
    vec3 waveColor = accentColor + vec3(0.5);
    
    finalColor += waveColor * wave * fade * 0.6;

    gl_FragColor = vec4(finalColor, 1.0);
}