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

const float HEX_SCALE = 42.0; 
const float HEX_LINE_WIDTH = 0.08;
const float HEX_LINE_SHARP = 0.02;

// --- FUNKCJA LOSUJĄCA ---
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

    float globalT = (glPos.x - u_bodyPosX) / u_bodyWidth;
    vec3 accentColor = mix(C_LEFT, C_RIGHT, clamp(globalT, 0.0, 1.0));

    vec2 p = glPos - u_pos;
    
    // --- GEOMETRIA ---
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
        finalColor += accentColor * 0.1 * smoothstep(u_size.y, 0.0, p.y);
        borderMask = max(borderMask, smoothstep(2.0, 0.0, p.y));
    } else {
        finalColor *= 0.6;
        if (u_hover) {
            borderMask += 0.3 * smoothstep(15.0, 0.0, dist);
            finalColor += vec3(0.1);
        }
    }

    finalColor = mix(finalColor, accentColor, clamp(borderMask, 0.0, 1.0));

    // --- LOGIKA REAKCJI ŁAŃCUCHOWEJ (DIGITAL DECAY) ---
    if (u_active) {
        vec2 globalHexUV = glPos / HEX_SCALE;
        vec2 cellVec = getHexCenterDist(globalHexUV);
        vec2 hexCenterPos = (globalHexUV - cellVec) * HEX_SCALE;
        
        float distToHex = distance(hexCenterPos, u_clickPos);

        // --- RANDOMIZACJA UNIKALNA DLA KLIKNIĘCIA ---
        // Dodajemy u_clickPos do seeda. Każde kliknięcie w innym miejscu da inny wzór.
        float rnd = hash(floor(hexCenterPos) + floor(u_clickPos)); 
        
        float jitterAmount = 60.0; 
        distToHex += (rnd - 0.5) * jitterAmount;

        float speed = 3000.0;
        float currentRadius = u_clickTime * speed;
        
        float diff = currentRadius - distToHex;
        
        float intensity = 0.0;

        if (diff > 0.0) {
            // --- OGRANICZENIE ZASIĘGU ---
            // 1. Zwiększyłem współczynnik z 0.0020 na 0.0060 (3x szybszy zanik zasięgu)
            float dropoffFactor = 0.0060;
            
            // 2. Odejmujemy 0.1 od wyniku. To sprawia, że szansa spada do zera 
            // ZANIM exp naturalnie osiągnie zero (co trwa w nieskończoność).
            // To eliminuje "pojedyncze heksy" daleko od centrum.
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
        
        vec3 hexGlow = accentColor * 2.5; 
        
        vec3 hexEffect = hexGlow * hexGridMask * intensity;
        hexEffect += accentColor * 0.4 * intensity * hexVal; 

        hexEffect *= (1.0 - clamp(borderMask, 0.0, 1.0));

        finalColor += hexEffect;
    }

    gl_FragColor = vec4(finalColor, 1.0);
}