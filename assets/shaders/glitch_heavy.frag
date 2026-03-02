#version 120

uniform sampler2D texture;

uniform float u_time;
uniform float u_intensity;
uniform float u_excludeRate; 

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float noise(float time) {
    float i = floor(time);
    float f = fract(time);
    return mix(random(vec2(i)), random(vec2(i + 1.0)), f);
}

void main() {
    vec2 uv = gl_TexCoord[0].xy;

    if (u_intensity <= 0.01) {
        gl_FragColor = texture2D(texture, uv) * gl_Color;
        return;
    }

    float numSlices = 60.0;
    float speed = 20.0;
    float maxShiftX = 0.05; 
    float rgbSplit = 0.01;  

    float sliceY = floor(uv.y * numSlices) / numSlices;

    float sliceRnd = random(vec2(sliceY, floor(u_time * 20.0)));

    float isSkipped = step(sliceRnd, u_excludeRate); 

    float timeStep = floor(u_time * speed);
    float randomValue = random(vec2(sliceY, timeStep));
    float sliceOffset = (randomValue - 0.5) * 2.0; 
    
    float flicker = noise(u_time * 15.0);
    float currentIntensity = u_intensity * (0.7 + 0.3 * flicker);
    
    float finalShiftX = sliceOffset * maxShiftX * currentIntensity;
    float splitX = rgbSplit * currentIntensity * 3.0;

    vec2 rUV = clamp(uv + vec2(finalShiftX - splitX, 0.0), 0.0, 1.0);
    vec2 bUV = clamp(uv + vec2(finalShiftX + splitX, 0.0), 0.0, 1.0);

    float r_glitch = texture2D(texture, rUV).a; 
    float b_glitch = texture2D(texture, bUV).a; 

    float originalAlpha = texture2D(texture, uv).a;

    float r = mix(r_glitch, originalAlpha, isSkipped);
    float b = mix(b_glitch, originalAlpha, isSkipped);
    float g = mix(0.0, originalAlpha, isSkipped);

    vec3 color = vec3(r, g, b);
    
    float finalAlpha = max(max(r, b), g);

    float scanLine = sin(uv.y * 800.0) * 0.1 * currentIntensity * (1.0 - isSkipped);
    color -= scanLine;

    gl_FragColor = vec4(color, finalAlpha) * gl_Color;
}