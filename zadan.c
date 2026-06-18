#include <math.h>
#include <stdio.h>

#define W 1650
#define H 1050
#define SEGMENTS 500
#define M_PI 3.1415926535f

// Структуры данных для работы с геометрией и цветом
struct vec2 { float x, y; };
struct rgb { float r, g, b; };

// Функция для генерации "радужного" градиента вдоль кривой
struct rgb get_gradient_color(float t) {
    return (struct rgb){
        0.5f + 0.5f * cosf(6.0f * t + 0.0f), 
        0.5f + 0.5f * cosf(6.0f * t + 2.0f), 
        0.5f + 0.5f * cosf(6.0f * t + 4.0f)
    };
}

// Функция определения траектории (органическая волна)
struct vec2 get_curve(float t) {
    float x = t;
    // Сложная синусоидально-косинусная модуляция
    float y = 0.5f + 0.3f * sinf(t * M_PI * 4.0f) * cosf(t * M_PI * 2.0f);
    return (struct vec2){x, y};
}

int main() {
    // Заголовок формата PPM (P3 - ASCII цвет)
    printf("P3\n%d %d\n255\n", W, H);
    
    // Предварительный расчет точек кривой для ускорения основного цикла
    struct vec2 curve_points[SEGMENTS];
    for (int k = 0; k < SEGMENTS; ++k) {
        curve_points[k] = get_curve((float)k / SEGMENTS);
    }
    
    // Отрисовка каждого пикселя изображения
    for (int j = 0; j < H; ++j) {
        for (int i = 0; i < W; ++i) {
            float px = (float)i / W;
            float py = (float)j / H;
            
            float min_dist = 1.0f;
            int best_segment = 0;

            // Поиск ближайшего сегмента кривой
            for (int k = 0; k < SEGMENTS; ++k) {
                float dx = px - curve_points[k].x;
                float dy = py - curve_points[k].y;
                float dist = sqrtf(dx*dx + dy*dy);
                
                if (dist < min_dist) {
                    min_dist = dist;
                    best_segment = k;
                }
            }
            
            // Расчет интенсивности неонового свечения
            // Формула: яркость падает обратно пропорционально расстоянию
            float glow = 0.015f / (min_dist + 0.01f);
            
            // Получение цвета для данной точки кривой
            struct rgb line_col = get_gradient_color((float)best_segment / SEGMENTS);
            
            // Итоговый цвет (фон темно-синий)
            float r = 0.02f + line_col.r * glow;
            float g = 0.02f + line_col.g * glow;
            float b = 0.05f + line_col.b * glow;
            
            // Ограничение значений до [0, 1] и вывод
            printf("%d %d %d\n", 
                (int)(fminf(r, 1.0f) * 255), 
                (int)(fminf(g, 1.0f) * 255), 
                (int)(fminf(b, 1.0f) * 255));
        }
    }
    return 0;
}