#version 120

// =================================================================================
// === SEKCJA KONFIGURACJI (TUTAJ ZMIENIASZ WYGLĄD WSZYSTKIEGO) ===
// =================================================================================

// Kolory
const vec3 C_LEFT  = vec3( 1.0, 0.0, 0.85 );
const vec3 C_RIGHT = vec3( 0.0, 0.8, 1.0 );
const vec3 C_BG    = vec3( 0.05, 0.05, 0.08 );

// Geometria ramek
const float BORDER_WIDTH_BODY = 3.0; // Grubosc ramki okna
const float BORDER_WIDTH_TAB  = 3.5; // Grubosc ramki zakladki

// Ustawienia Hexów
const float HEX_SCALE      = 42.0; 
const float HEX_LINE_WIDTH = 0.08;
const float HEX_LINE_SHARP = 0.02;

// --- USTAWIENIA ANIMACJI I WYGLĄDU FALI ---

// 1. GLOBALNA JASNOŚĆ EFEKTU (To o co prosiłeś)
// 1.0 = Pełna moc (tak jak było wcześniej).
// 0.5 = Przygaszone do połowy.
// 2.0 = Bardzo jaskrawe (overblown).
const float HEX_BRIGHTNESS = 0.3; 

// 2. SZYBKOŚĆ
const float ANIM_SPEED     = 3000.0;

// 3. EASING (Dynamika)
const float EASING_POWER   = 0.75; // <1.0 = Szybki start (wybuch), 1.0 = Liniowo

// 4. JITTER_AMOUNT (Krzywizna krawędzi)
const float JITTER_AMOUNT  = 60.0;   

// 5. HOLE_STRENGTH (Dziury w środku)
const float HOLE_STRENGTH  = 0.1; 

const float DROPOFF_FACTOR = 0.0010; // Jak szybko znika zasieg
const float DECAY_SPEED    = 0.008;  // Jak szybko wygasaja

// =================================================================================
// === KONIEC KONFIGURACJI ===
// =================================================================================

uniform int u_type; // 0 = BODY, 1 = TAB

uniform vec2 u_pos;       // Lokalna pozycja
uniform vec2 u_size;      // Rozmiar elementu
uniform float u_winHeight;// Wysokosc okna

// Zmienne specyficzne dla TAB
uniform float u_skew;
uniform float u_bodyPosX;
uniform float u_bodyWidth;
uniform bool u_active;
uniform bool u_hover;

// Zmienne interakcji
uniform vec2 u_clickPos;
uniform float u_clickTime;

// --- FUNKCJE POMOCNICZE ---

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

    vec3 finalColor = C_BG;
    float borderMask = 0.0;
    vec3 accentColor = vec3(0.0);
    bool applyHex = false;

    // =============================================================================
    // === LOGIKA GEOMETRII ===
    // =============================================================================
    
    if (u_type == 0) { 
        // BODY
        float t = clamp(p.x / u_size.x, 0.0, 1.0);
        accentColor = mix(C_LEFT, C_RIGHT, t);
        float distInner = min(p.x, u_size.x - p.x);
        distInner = min(distInner, min(p.y, u_size.y - p.y));
        borderMask = 1.0 - smoothstep(0.0, 1.5, distInner - BORDER_WIDTH_BODY);
        finalColor = mix(finalColor, accentColor, borderMask);
        applyHex = true; 
    } 
    else {
        // TAB
        float globalT = (glPos.x - u_bodyPosX) / u_bodyWidth;
        accentColor = mix(C_LEFT, C_RIGHT, clamp(globalT, 0.0, 1.0));
        float t = clamp(p.y / u_size.y, 0.0, 1.0);
        float leftLimit = u_skew * (1.0 - t);
        float rightLimit = u_size.x - (u_skew * (1.0 - t));
        float dLeft = p.x - leftLimit;
        float dRight = rightLimit - p.x;
        float dTop = p.y;
        float dist;
        if (u_active) dist = min(min(dLeft, dRight), dTop);
        else {
            float dBot = u_size.y - p.y;
            dist = min(min(dLeft, dRight), min(dTop, dBot));
        }

        if (dist < -1.0) discard;
        borderMask = smoothstep(BORDER_WIDTH_TAB + 0.8, BORDER_WIDTH_TAB - 0.8, dist);

        if (u_active) {
            finalColor = C_BG;
            finalColor += accentColor * 0.1 * smoothstep(u_size.y, 0.0, p.y);
            borderMask = max(borderMask, smoothstep(2.0, 0.0, p.y));
            applyHex = true; 
        } else {
            finalColor *= 0.6;
            if (u_hover) {
                borderMask += 0.3 * smoothstep(15.0, 0.0, dist);
                finalColor += vec3(0.1);
            }
            applyHex = false;
        }
        finalColor = mix(finalColor, accentColor, clamp(borderMask, 0.0, 1.0));
    }

    // =============================================================================
    // === LOGIKA EFEKTU HEX ===
    // =============================================================================

    if (applyHex) {
        vec2 globalHexUV = glPos / HEX_SCALE;
        vec2 cellVec = getHexCenterDist(globalHexUV);
        vec2 hexCenterPos = (globalHexUV - cellVec) * HEX_SCALE;
        
        float distToHex = distance(hexCenterPos, u_clickPos);

        float rnd = hash(floor(hexCenterPos) + floor(u_clickPos)); 
        
        // Jitter (Kształt)
        distToHex += (rnd - 0.5) * JITTER_AMOUNT;

        // Easing (Dynamika)
        float currentRadius = pow(u_clickTime, EASING_POWER) * ANIM_SPEED;
        
        float diff = currentRadius - distToHex;
        float intensity = 0.0;

        if (diff > 0.0) {
            float survivalChance = exp(-distToHex * DROPOFF_FACTOR) - 0.1;
            
            // Dziury (Holes)
            if (rnd * HOLE_STRENGTH <= survivalChance) {
                 float decay = exp(-diff * DECAY_SPEED); 
                 if (diff < 800.0) {
                     intensity = decay;
                 }
            }
        }

        float hexVal = 0.5 - hexDist(cellVec);
        float hexGridMask = smoothstep(HEX_LINE_WIDTH, HEX_LINE_SHARP, hexVal);
        
        vec3 waveColor = accentColor;
        if (u_type == 0) waveColor += vec3(0.4); 
        else waveColor *= 2.5;                   

        vec3 hexEffect = waveColor * hexGridMask * intensity;
        hexEffect += accentColor * 0.4 * intensity * hexVal;

        // --- ZASTOSOWANIE GLOBALNEJ JASNOŚCI ---
        hexEffect *= HEX_BRIGHTNESS;

        hexEffect *= (1.0 - clamp(borderMask, 0.0, 1.0));

        finalColor += hexEffect;
    }

    gl_FragColor = vec4(finalColor, 1.0);
}