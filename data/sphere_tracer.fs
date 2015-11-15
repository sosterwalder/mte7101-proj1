#version 110

uniform vec2 globalResolution;
uniform float globalTime;

#define SHOW_DISTANCE false

float hash(float n) {
    return fract(sin(n)*33753.545383);
}

mat3 calcLookAtMatrix(vec3 origin, vec3 target, float roll) {
    vec3 rr = vec3(sin(roll), cos(roll), 0.0);
    vec3 ww = normalize(target - origin);
    vec3 uu = normalize(cross(ww, rr));
    vec3 vv = normalize(cross(uu, ww));

    return mat3(uu, vv, ww);
}

vec3 getRay(mat3 cameraMatrix, vec2 screenPosition, float lensLength) {
    return normalize(cameraMatrix * vec3(screenPosition, lensLength));
}

vec3 getRay(vec3 origin, vec3 target, vec2 screenPosition, float lensLength) {
    mat3 cameraMatrix = calcLookAtMatrix(origin, target, 0.0);

    return getRay(cameraMatrix, screenPosition, lensLength);
}


vec2 squareFrame(vec2 screenSize) {
    vec2 position = 2.0 * (gl_FragCoord.xy / screenSize.xy) - 1.0;
    position.x *= screenSize.x / screenSize.y;

    return position;
}

vec2 squareFrame(vec2 screenSize, vec2 coord) {
    vec2 position = 2.0 * (coord.xy / screenSize.xy) - 1.0;
    position.x *= screenSize.x / screenSize.y;

    return position;
}

float plane(vec3 position)
{
    return position.y;
}

float sphere(vec3 position, float radius)
{
    return length(position) - radius;
}

float box(vec3 position, vec3 dimension)
{
    position = abs(position) - dimension;
    return max(max(position.x, position.y), position.z);
}

float ruler(in vec3 position, in float rulerOffset)
{
    return abs(position.y) - rulerOffset;
}


float subtract(float a, float b)
{
    return max(-b, a);
}

float merge(float a, float b)
{
    return min(a, b);
}

float intersect(float a, float b)
{
    return (a > b) ? a : b;
}

float scene1(in vec3 position)
{
    float result = 0.0;

    float sphereRadius   = 0.6;
    float sphere3Radius  = 1.0;
    vec3  boxDimension   = vec3(0.8);

    vec3 sphereOffset  = vec3(-2.0, 0.8, -2.0);
    vec3 boxOffset     = vec3(-3.0, 0.8,  0.0);
    vec3 box2Offset    = vec3(-2.0, 0.8, -2.0);

    float plane = plane(position - vec3(-1.0));

    float intersection = intersect(
        box(position - boxOffset, vec3(0.8)),
        sphere(position - boxOffset, 1.1)
    );

    float subtraction = subtract(
        box(position - box2Offset, vec3(0.8)),
        sphere(position - box2Offset, sphere3Radius)
    );

    if (SHOW_DISTANCE) {
        result = merge(
            sphere(position - sphereOffset, sphereRadius),
            intersection
        );
    }
    else {
        result = merge(
            plane,
            sphere(position - sphereOffset, sphereRadius)
        );
        result = merge(
            result,
            intersection
        );
    }

    result = merge(
        result,
        subtraction
    );

    return result;
}

float testScene(in vec3 position)
{
    float result = 0.0;

    float sphere = sphere(position - vec3(0.0, 1.0, 0.0), 1.0);
    float plane = plane(position);

    result = merge(
        plane, sphere
    );

    // result = sphere;

    return result;
}

vec3 combineScenes(vec3 p)
{
    float scene     = scene1(p);
    float ruler     = ruler(p, 0.01);
    float showRuler = ruler < scene ? 1.0 : 0.0;

    return vec3(min(scene, ruler), showRuler, scene);
}

float calcShadows(in vec3 rayOrigin, in vec3 rayDirection)
{
    float shadow            = 1.0;
    float minimalDistance   = 0.02;
    float maximalDistance   = 2.5;
    float convergePrecision = 0.000001;
    float kShadow           = 8.0;
    float currentDistance   = minimalDistance;

    while (currentDistance < maximalDistance) {
        vec3 ray = rayOrigin + rayDirection * currentDistance;
        float estimatedDistance = scene1(ray);

        if (estimatedDistance < convergePrecision) {
            return 0.0;
        }

        float penumbraFactor = estimatedDistance / currentDistance;
        shadow = min(shadow, kShadow * penumbraFactor);
        currentDistance += estimatedDistance;
    }

    return clamp(shadow, 0.0, 1.0);

}

vec3 calcBRDFLighting(in vec3 position, in vec3 normal, in vec3 rayDirection, in vec3 material, in vec3 lightPosition, in vec3 lightColor, in float currentDistance)
{
    vec3 lightDirection     = normalize(lightPosition);
    vec3 reflection         = reflect(rayDirection, normal);

    lightColor              = 0.45 + 0.3 * sin(vec3(0.05, 0.08, 0.10) * (currentDistance - 1.0));

    if (currentDistance < 1.5) {
        float f = mod(floor(5.0 * lightDirection.z) + floor(5.0 * lightDirection.x), 2.0);
        lightColor = 0.4 + 0.1 * f * vec3(1.0);
    }

    vec3  ambientColor      = vec3(0.5, 0.7, 1.0);
    vec3  diffuseColor      = vec3(1.0, 0.9, 0.6);
    vec3  specularColor     = vec3(1.0, 0.9, 0.6);
    vec3  domColor          = vec3(0.5, 0.7, 1.0);
    vec3  bacColor          = vec3(0.25, 0.25, 0.25);
    vec3  fresnelColor      = vec3(1.0);

    float kOcclusion        = 0.0;
    float kAmbient          = clamp(0.5 + 0.5 * normal.y, 0.0, 1.0);
    float kDiffuse          = clamp(dot(lightDirection, normal), 0.0, 1.0);
    float kBac              = clamp(dot(normal, normalize(vec3(-lightDirection.x, 0.0, -lightDirection.z))), 0.0, 1.0) * clamp(1.0 - position.y, 0.0, 1.0);
    float kDom              = smoothstep(-0.1, 0.1, reflection.y);
    float kFresnel          = pow(clamp(1.0 + dot(normal, rayDirection), 0.0, 1.0), 2.0);
    float kSpecular         = pow(clamp(dot(reflection, lightDirection), 0.0, 1.0), 16.0);

    kDiffuse               *= calcShadows(position, lightDirection);
    kDom                   *= calcShadows(position, reflection);

    vec3 brdf               = vec3(0.0);
    brdf                   += 1.20 * kDiffuse  * diffuseColor;
    brdf                   += 1.20 * kSpecular * specularColor * kDiffuse;
    brdf                   += 0.30 * kAmbient  * ambientColor  * kOcclusion;
    brdf                   += 0.40 * kDom      * domColor      * kOcclusion;
    brdf                   += 0.30 * kBac      * bacColor      * kOcclusion;
    brdf                   += 0.40 * kFresnel  * fresnelColor  * kOcclusion;
    brdf                   += 0.02;

    lightColor             = lightColor * brdf * material;
    lightColor             = mix(lightColor, vec3(0.8, 0.9, 1.0), 1.0 - exp(-0.0005 * currentDistance * currentDistance));

    return lightColor;
}

vec3 calcLighting(in vec3 position, in vec3 normal, in vec3 rayDirection, in vec3 material, in vec3 lightPosition, in vec3 lightColor)
 {
     vec3 lightDirection     = normalize(lightPosition);
 
     float kDirectLight      = 0.1;
     float shadows           = calcShadows(position, lightDirection);
     vec3  direct            = vec3(kDirectLight * shadows);
 
     vec3  ambientColor      = vec3(0.3, 0.3, 0.3);
     float kAmbient          = clamp(0.5 + 0.5 * normal.y, 0.0, 1.0);
     vec3  ambient           = kAmbient * ambientColor;
 
     vec3  diffuseColor      = vec3(lightColor);
     float kDiffuse          = clamp(dot(lightDirection, normal), 0.0, 1.0);
     vec3  diffuse           = kDiffuse * diffuseColor;
 
     vec3  specularColor     = vec3(1.0);
     float kSpecularExponent = 24.0;
     vec3  h                 = normalize(-rayDirection + lightDirection);
     float nFacing           = clamp(dot(lightDirection, normal), 0.0, 1.0);
     float kSpecular         = pow(clamp(dot(h, normal), 0.0, 1.0), kSpecularExponent);
     vec3  specular          = nFacing * kSpecular * specularColor;
 
     vec3 light              = ambient + diffuse + specular + direct;
     vec3 color              = material * light;
 
     return color;
 }

vec3 calcMaterial(in vec3 position, in vec3 normal)
{
    return vec3(1.0);
}

vec3 calcNormal(in vec3 position, in float epsilon)
{
    const vec3 v1 = vec3( 1.0, -1.0, -1.0);
    const vec3 v2 = vec3(-1.0, -1.0,  1.0);
    const vec3 v3 = vec3(-1.0,  1.0, -1.0);
    const vec3 v4 = vec3( 1.0,  1.0,  1.0);

    vec3 sceneAtV1 = combineScenes(position + v1 * epsilon);
    vec3 sceneAtV2 = combineScenes(position + v2 * epsilon);
    vec3 sceneAtV3 = combineScenes(position + v3 * epsilon);
    vec3 sceneAtV4 = combineScenes(position + v4 * epsilon);

    float currentDistanceAtV1 = -1.0;
    float currentDistanceAtV2 = -1.0;
    float currentDistanceAtV3 = -1.0;
    float currentDistanceAtV4 = -1.0;


    if (SHOW_DISTANCE) {
        currentDistanceAtV1 = sceneAtV1.x;
        currentDistanceAtV2 = sceneAtV2.x;
        currentDistanceAtV3 = sceneAtV3.x;
        currentDistanceAtV4 = sceneAtV4.x;
    }
    else {
        currentDistanceAtV1 = sceneAtV1.z;
        currentDistanceAtV2 = sceneAtV2.z;
        currentDistanceAtV3 = sceneAtV3.z;
        currentDistanceAtV4 = sceneAtV4.z;
    }

    return normalize(v1 * currentDistanceAtV1 +
                     v2 * currentDistanceAtV2 +
                     v3 * currentDistanceAtV3 +
                     v4 * currentDistanceAtV4);
}

float calcRule(in float currentDistance, in float scale) {
    return mix(1.0, 0.35, smoothstep(0.6, 1.0, abs(fract(currentDistance * scale) * 2.0 - 1.0)));
}

vec3 calcRulerColor(float stepSize) {
    stepSize = clamp(log(stepSize + 1.0), 0.0, 1.0);

    return mix(mix(vec3(0.0, 0.1, 1.0), vec3(1.0, 0.1, 0.0), stepSize * 5.0), vec3(1.0), smoothstep(0.2, 0.5, stepSize));
}

vec3 calcRulerMaterial(float distanceToScene, float currentDistance) {
    float limit = log(currentDistance) / log(10.0);
    float stepSize = pow(10.0, -floor(limit));
    float scale = smoothstep(0.0, 0.33, fract(limit));
    float interpolatedRule = calcRule(distanceToScene, stepSize * 10.0) * mix(calcRule(distanceToScene, stepSize * 100.0), calcRule(distanceToScene, stepSize), scale);

    return mix(calcRulerColor(stepSize * distanceToScene), calcRulerColor(stepSize * distanceToScene * 0.1), scale) * 0.8 * interpolatedRule;
}

vec3 calcPostFx(vec3 color, vec2 screenPosition)
{
    float contrast   = 1.0;
    float saturation = 1.7;
    float brightness = 1.2;

    // Gamma correction
    color = pow(color, vec3(0.4545));

    // color = mix(vec3(.5), mix(vec3(dot(vec3(.2125, .7154, .0721), color*brightness)), color*brightness, saturation), contrast);

    // Noise
    // float noiseFactor = hash(globalTime);
    // vec2 noise        = noiseFactor * screenPosition;
    // color             = clamp(color + noise * 0.1, 0.0, 1.0);

    // Vignetting
    // color *= .4+0.5*pow(40.0*screenPosition.x*screenPosition.y*(1.0-screenPosition.x)*(1.0-screenPosition.y), 0.2 );

    return color;
}

vec3 castRay(in vec3 rayOrigin, in vec3 rayDirection, in float maxDistance, in float precision, in int steps)
{
    float latest          = precision * 2.0;
    float currentDistance = 0.0;
    float result          = -1.0;
    vec3  ray             = vec3(0);
    float ruler           = 0.0;
    vec3 combinedScenes   = vec3(-1.0);

    for(int i = 0; i < steps; i++) {
        if (abs(latest) < precision || currentDistance > maxDistance) {
            continue;
        }

        ray = rayOrigin + rayDirection * currentDistance;
        combinedScenes = combineScenes(ray);

        if (SHOW_DISTANCE) {
            latest = combinedScenes.x;
        }
        else {
            latest = combinedScenes.z;
        }

        currentDistance += latest;
    }

    if (currentDistance < maxDistance) {
        result = currentDistance;
    }

    float showRuler     = combinedScenes.y;
    float renderedScene = combinedScenes.z;

    return vec3(result, showRuler, renderedScene);
}

vec3 render(in vec3 rayOrigin, in vec3 rayDirection)
{
    vec3  color           = vec3(0.05, 0.08, 0.10);
    vec3  res             = castRay(rayOrigin, rayDirection, 100.0, 0.00001, 100);
    float currentDistance = res.x;
    bool  showRuler       = res.y == 1.0;
    float renderedScene   = res.z;

    if (currentDistance > -0.5) {
        vec3 position = rayOrigin + currentDistance * rayDirection;
        vec3 normal   = calcNormal(position, 0.000001);

        vec3 material = vec3(0.8);

        if (SHOW_DISTANCE) {
            if (showRuler) {
                material = calcRulerMaterial(renderedScene, currentDistance);
            }
            else {
                material = calcMaterial(position, normal);
            }
        }
        else {
            material = calcMaterial(position, normal);
        }

        vec3 light1Color = vec3(0.7, 0.2, 0.3);
        vec3 light1Position = vec3(-0.6, 0.7, -0.5);
        // vec3 light1 = calcBRDFLighting(position, normal, rayDirection, material, light1Position, light1Color, currentDistance);
        vec3 light1 = calcLighting(position, normal, rayDirection, material, light1Position, light1Color);

        vec3 light2Color = vec3(0.3, 0.2, 0.8);
        vec3 light2Position = vec3(1.3, 0.2, 0.8);
        // vec3 light2 = calcBRDFLighting(position, normal, rayDirection, material, light2Position, light2Color, currentDistance);
        vec3 light2 = calcLighting(position, normal, rayDirection, material, light2Position, light2Color);

        color = light1 + light2;
    }
    color      = clamp(color, 0.0, 1.0);

    return color;
}

void main()
{
    vec2 resolution      = globalResolution;
    float time           = 1.0; // globalTime * 1.1;
    float cameraAngle    = 1.0;
    float cameraHeight   = 2.0;
    float cameraPane     = 4.0;
    float cameraDistance = 4.5;
    vec3 rayOrigin      = vec3(cameraPane * sin(cameraAngle), cameraHeight, cameraDistance * cos(cameraAngle * time));
    vec3 rayTarget      = vec3(0.0, 0.0, 0.0);
    vec2 screenPosition = squareFrame(resolution);
    vec3 rayDirection   = getRay(rayOrigin, rayTarget, screenPosition, 2.0);

    vec3 color          = render(rayOrigin, rayDirection);
    color               = calcPostFx(color, screenPosition);

    gl_FragColor.rgb    = color;
    gl_FragColor.a      = 1.0;
}
