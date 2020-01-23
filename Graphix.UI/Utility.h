#pragma once

float radians(float degrees) {
return (180 / DirectX::XM_PI) * degrees;
}

float degrees(float radians) {
return (DirectX::XM_PI / 180) * radians;
}

