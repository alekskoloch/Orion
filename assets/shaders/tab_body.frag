#version 120

uniform vec2 u_size;
uniform vec2 u_pos;
uniform float u_winHeight;

uniform vec2 u_clickPos;
uniform float u_clickTime;

const float BORDER_WIDTH = 3.0;
const vec3 C_LEFT  = vec3( 1.0, 0.0, 0.85 );
const vec3 C_RIGHT = vec3( 0.0, 0.8, 1.0 );
const vec3 C_BG    = vec3( 0.05, 0.05, 0.08 );

void main() {
    vec2 glPos = gl_FragCoord.xy;
    glPos.y = u_winHeight - glPos.y;
    vec2 p = glPos - u_pos;

    float t = clamp(p.x / u_size.x, 0.0, 1.0);
    vec3 borderColor = mix(C_LEFT, C_RIGHT, t);

    float distInner = min(p.x, u_size.x - p.x);
    distInner = min(distInner, min(p.y, u_size.y - p.y));
    float borderMask = 1.0 - smoothstep(0.0, 1.5, distInner - BORDER_WIDTH);

    vec3 finalColor = C_BG;
    finalColor = mix(finalColor, borderColor, borderMask);

    float distClick = distance(glPos, u_clickPos);
    
    float speed = 2500.0;
    float radius = u_clickTime * speed;
    
    float waveWidth = 150.0;
    
    float wave = smoothstep(radius, radius - waveWidth, distClick) 
               - smoothstep(radius - waveWidth * 0.2, radius - waveWidth * 1.2, distClick);
    wave = max(0.0, wave);

    float fade = exp(-u_clickTime * 4.0);
    
    vec3 waveColor = borderColor + vec3(0.5);
    
    finalColor += waveColor * wave * fade * 0.6;

    gl_FragColor = vec4(finalColor, 1.0);
}