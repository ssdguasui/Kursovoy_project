#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define MAX_MATERIALS 50



// Структура, представляющая материал
struct materialroom {
    char name[50];    // Название материала
    double length;    // Длина материала
    double width;     // Ширина материала
    double square;    // Площадь материала
};

// Структура, представляющая параметры комнаты
struct room {
    int S;                 // Площадь комнаты
    int height;            // Высота комнаты
    int length1;           // Длина стены 1
    int length2;           // Длина стены 2
    int area_of_openings;  // Площадь проемов комнаты
    int S_openings;        // Площадь комнаты с учётом проёмов
    int rolls;             // Кол-во рулонов
    int glue;              // Кол-во клея
    int window_count;      // Количество окон
    int door_count;        // Количество дверей
    int window_width;      // Ширина окна
    int window_length;     // Длина окна
    int door_width;        // Ширина двери
    int door_length;       // Длина двери
};



// Прототипы функций
struct materialroom* read_materials_file(char* fname, int* materialCount);
int set_room_parameters(struct room* room);
int set_openings_parameters(struct room* room);
int output_of_the_result_with_openings(struct room* room);
int number_of_rolls(struct room* room);
int output_of_glue_amount(struct room* room);
void write_results_to_file(struct room* room);



int main() {
    setlocale(LC_ALL, "rus");

    int variant = 0;
    struct room room_data;
    int materialCount;
    char* fname = "materials.txt";
    struct materialroom* materials = read_materials_file(fname, &materialCount);
    int marerialls;

    do {
        system("cls");
        printf("\n * Программа расчета расхода шумоизоляционного материала *\n\n\n");
        printf("* Важно! Все расчеты введутся в сантиметрах *\n");
        printf("1. Выбор материала\n");
        printf("2. Задать параметры комнаты\n");
        printf("3. Задать параметры проемов комнаты\n");
        printf("4. Расчет\n");
        printf("5. Кол-во клея\n");
        printf("6. Отчёт в файле\n");
        printf("10. Завершить программу\n");
        printf("Выберите опцию: ");
        scanf("%d", &variant);

        switch (variant) {
        case 1:
            // Вывод доступных материалов
            read_materials_file(fname, &materialCount);

            for (int i = 0; i < materialCount; i++) {
                printf("%d. %s\n", i + 1, materials[i].name);
                printf("Длина на ширину - %1.lfx%1.lf\nПлощадь - %.1lf\n", materials[i].length, materials[i].width, materials[i].square);
            }
            scanf("%d", &marerialls);
            break;

        case 2:
            // Задание параметров комнаты
            room_data.S = set_room_parameters(&room_data);
            break;

        case 3:
            // Задание параметров проемов комнаты
            room_data.area_of_openings = set_openings_parameters(&room_data);
            break;

        case 4:
            // Расчет и вывод результатов
            if (room_data.height > 0 && room_data.length1 > 0 && room_data.length2 > 0) {
                printf("Площадь стен комнаты без проёмов = %d см^2 (%.2lf м^2)\n", room_data.S, room_data.S / 10000.);

                if (room_data.area_of_openings > room_data.S) {
                    printf("\nВы ввели некорректное значение, площадь проёмов больше площади комнаты.\n Запустите программу заново!\n\n");
                }
                else {
                    room_data.S_openings = output_of_the_result_with_openings(&room_data);
                    printf("\nПлощадь всех стен с учётом проёмов = %d см^2 (%.2lf м^2)\n", room_data.S_openings, room_data.S_openings / 10000.);
                    room_data.rolls = number_of_rolls(&room_data);
                    printf("Вам потребуется %d рулонов, чтобы покрыть комнату шумоизоляционным материалом", room_data.rolls);
                }
            }
            else {
                printf("\nОшибка: Пожалуйста, сначала задайте параметры комнаты.\n");
            }

            printf("\nПродолжить?. Любое число-продолжить, 10-закончить\n");
            scanf("%d", &variant);
            break;
        case 5:
            // Вывод количества клея
            room_data.glue = output_of_glue_amount(&room_data);
            printf("Вам потребуется %d мл клея (%.2lf литров)\n", room_data.glue, room_data.glue / 10000.);
            printf("\nПродолжить?. Любое число-продолжить, 10-закончить\n");
            scanf("%d", &variant);
            break;

        case 6:
            // Запись результатов в файл
            if (room_data.height > 0 && room_data.length1 > 0 && room_data.length2 > 0) {
                write_results_to_file(&room_data);
                printf("\nОтчет сохранен в файле 'results.txt'\n");
            }
            else {
                printf("\nОшибка: Пожалуйста, сначала задайте параметры комнаты.\n");
            }
            printf("\nПродолжить?. Любое число-продолжить, 10-закончить\n");
            scanf("%d", &variant);
            break;

        case 10:
            break;
        }

    } while (variant != 10);

    return 0;
}

// Функция чтения материалов из файла
struct materialroom* read_materials_file(char* fname, int* materialCount) {
    FILE* file = fopen(fname, "r");
    if (!file) {
        puts("Не получилось открыть файл");
        return 0;
    }

    struct materialroom* materials = malloc(MAX_MATERIALS * sizeof(struct materialroom));
    if (!materials) {
        puts("Не получилось выделить память для массива структур");
        return 0;
    }

    int count = 0;
    char ly[1000];

    while (fgets(ly, sizeof(ly), file) != NULL) {
        // Извлечение названия материала
        for (int i = 0; i < strlen(ly); i++) {
            if (ly[2 + i] == ' ' && ly[2 + i + 1] == ' ') {
                materials[count].name[i] = '\0';
                break;
            }
            materials[count].name[i] = ly[2 + i];
        }

        char buf[1000];

        // Извлечение параметров материала
        for (int i = 0; i < strlen(ly); i++) {
            buf[i] = ly[30 + i];
        }

        sscanf(buf, "|    | %lfx%lf |    |%lf м^2|", &materials[count].length, &materials[count].width, &materials[count].square);

        count++;
    }

    fclose(file);
    *materialCount = count;
    return materials;
}

// Функция задания параметров комнаты
int set_room_parameters(struct room* room) {
    printf("\nВведите высоту комнаты\n");
    printf("Высота комнаты = ");
    scanf("%d", &(room->height));

    printf("\nВведите длину одной стены комнаты\n");
    printf("Длина одной стены комнаты = ");
    scanf("%d", &(room->length1));

    printf("\nВведите длину другой стены комнаты\n");
    printf("Длина другой стены комнаты = ");
    scanf("%d", &(room->length2));

    // Расчет площади стен комнаты без проемов
    return (room->length1 * room->height + room->length2 * room->height) * 2;
}

// Функция задания параметров проемов комнаты
int set_openings_parameters(struct room* room) {
    printf("\nВведите количество окон в комнате:\n");
    scanf("%d", &(room->window_count));

    printf("\nВведите размеры окна (ширина и длина):\n");
    scanf("%d%d", &(room->window_width), &(room->window_length));

    printf("\nВведите количество дверей в комнате:\n");
    scanf("%d", &(room->door_count));

    printf("\nВведите размеры двери (ширина и длина):\n");
    scanf("%d%d", &(room->door_width), &(room->door_length));

    return (room->window_count * room->window_width * room->window_length) + (room->door_width * room->door_length * room->door_count);
}

// Функция вывода площади стен с учетом проемов
int output_of_the_result_with_openings(struct room* room) {
    room->S_openings = room->S - room->area_of_openings;
    return room->S_openings;
}

// Функция расчета количества рулонов
int number_of_rolls(struct room* room) {
    room->rolls = ((room->S - room->area_of_openings) / 220) + 1;
    return room->rolls;
}

// Функция расчёта и вывода количества клея
int output_of_glue_amount(struct room* room) {
    room->glue = (room->S - room->area_of_openings) / 2.;
    write_results_to_file(room);
    return room->glue;
}

// Функция записи результатов в файл
void write_results_to_file(struct room* room) {
    FILE* file = fopen("results.txt", "w");

    if (file == NULL) {
        printf("Ошибка открытия файла для записи\n");
        return -1;
    }
    else {

        fprintf(file, "Высота комнаты: %d см (%.3lf м^2)\n", room->height, (room->height) / 10000.);
        fprintf(file, "Длина стены 1: %d см (%.3lf м^2)\n", room->length1, (room->length1) / 10000.);
        fprintf(file, "Длина стены 2: %d см (%.3lf м^2)\n", room->length2, (room->length2) / 10000.);
        fprintf(file, "Площадь всех стен без учёта проёмов = %d см^2 (%.2lf м^2)\n", room->S, room->S / 10000.);
        fprintf(file, "Количество окон: %d\n", room->window_count);
        fprintf(file, "Размеры окон: %dx%d см\n", room->window_width, room->window_length);
        fprintf(file, "Количество дверей: %d\n", room->door_count);
        fprintf(file, "Размеры дверей: %dx%d см\n", room->door_width, room->door_length);
        fprintf(file, "Площадь всех стен с учётом проёмов = %d см^2 (%.2lf м^2)\n", room->S_openings, room->S_openings / 10000.);
        fprintf(file, "Вам потребуется %d рулонов, чтобы покрыть комнату шумоизоляционным материалом\n", room->rolls);
        fprintf(file, "Вам потребуется %d мл клея (%.2lf литров)\n", room->glue, room->glue / 10000.);

        if (fclose(file) == 0) {
            return 1;
        }
        else {
            printf("Ошибка закрытия файла\n");
            return -1;
        }
    }
}
