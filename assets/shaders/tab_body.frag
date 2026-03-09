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

const float HEX_SCALE = 42.0;
const float HEX_LINE_WIDTH = 0.08;
const float HEX_LINE_SHARP = 0.02;

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

float hexDist(vec2 p) {
    p = abs(p);
    float c = dot(p, normalize(vec2(1.0, 1.732)));
    return max(c, p.x);
}

vec2 getHexCenterDist(vec2 p) {
    vec2 r = vec2(1.0, 1.732);
    vec2 h = r * 0.5;
    vec2 a = mod(p, r) - h;
    vec2 b = mod(p - h, r) - h;
    return dot(a, a) < dot(b, b) ? a : b;
}

void main() {
    vec2 glPos = gl_FragCoord.xy;
    glPos.y = u_winHeight - glPos.y;
    vec2 p = glPos - u_pos;

    float t = clamp(p.x / u_size.x, 0.0, 1.0);
    vec3 borderColor = mix(C_LEFT, C_RIGHT, t);

    // --- GEOMETRIA BODY ---
    float distInner = min(p.x, u_size.x - p.x);
    distInner = min(distInner, min(p.y, u_size.y - p.y));
    float borderMask = 1.0 - smoothstep(0.0, 1.5, distInner - BORDER_WIDTH);

    vec3 finalColor = C_BG;
    finalColor = mix(finalColor, borderColor, borderMask);

    // --- LOGIKA REAKCJI ŁAŃCUCHOWEJ (DIGITAL DECAY) ---
    
    vec2 globalHexUV = glPos / HEX_SCALE;
    vec2 cellVec = getHexCenterDist(globalHexUV);
    vec2 hexCenterPos = (globalHexUV - cellVec) * HEX_SCALE;
    
    float distToHex = distance(hexCenterPos, u_clickPos);
    
    // --- RANDOMIZACJA UNIKALNA ---
    // Dodajemy pozycję kliknięcia do generatora liczb losowych
    float rnd = hash(floor(hexCenterPos) + floor(u_clickPos)); 
    
    float jitterAmount = 60.0; 
    distToHex += (rnd - 0.5) * jitterAmount;
    
    float speed = 3000.0;
    float currentRadius = u_clickTime * speed;
    
    float diff = currentRadius - distToHex;
    
    float intensity = 0.0;

    if (diff > 0.0) {
        // --- OGRANICZENIE ZASIĘGU ---
        // Szybszy spadek szansy na przetrwanie (0.0060)
        float dropoffFactor = 0.0060;
        
        // Odjęcie 0.1 powoduje twarde odcięcie w pewnym momencie (brak "długiego ogona")
        float survivalChance = exp(-distToHex * dropoffFactor) - 0.1; 
        
        if (rnd <= survivalChance) {
             float decay = exp(-diff * 0.005); 
             if (diff < 800.0) {
                 intensity = decay;
             }
        }
    }

    float hexVal = 0.5 - hexDist(cellVec);
    float hexGridMask = smoothstep(HEX_LINE_WIDTH, HEX_LINE_SHARP, hexVal);
    
    vec3 waveColor = borderColor + vec3(0.4); 
    
    vec3 hexEffect = waveColor * hexGridMask * intensity;
    hexEffect += waveColor * 0.3 * intensity * hexVal;

    hexEffect *= (1.0 - borderMask);

    finalColor += hexEffect;

    gl_FragColor = vec4(finalColor, 1.0);
}