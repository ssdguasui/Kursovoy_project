#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define MAX_MATERIALS 50



// ���������, �������������� ��������
struct materialroom {
    char name[50];    // �������� ���������
    double length;    // ����� ���������
    double width;     // ������ ���������
    double square;    // ������� ���������
};

// ���������, �������������� ��������� �������
struct room {
    int S;                 // ������� �������
    int height;            // ������ �������
    int length1;           // ����� ����� 1
    int length2;           // ����� ����� 2
    int area_of_openings;  // ������� ������� �������
    int S_openings;        // ������� ������� � ������ ������
    int rolls;             // ���-�� �������
    int glue;              // ���-�� ����
    int window_count;      // ���������� ����
    int door_count;        // ���������� ������
    int window_width;      // ������ ����
    int window_length;     // ����� ����
    int door_width;        // ������ �����
    int door_length;       // ����� �����
};



// ��������� �������
struct materialroom* read_materials_file(char* fname, int* materialCount);
int set_room_parameters(struct room* room);
int set_openings_parameters(struct room* room);
void output_the_result_without_openings(struct room* room);
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
        printf("\n * ��������� ������� ������� ����������������� ��������� *\n\n\n");
        printf("* �����! ��� ������� �������� � ����������� *\n");
        printf("1. ����� ���������\n");
        printf("2. ������ ��������� �������\n");
        printf("3. ������ ��������� ������� �������\n");
        printf("4. ������\n");
        printf("5. ���-�� ����\n");
        printf("6. ����� � �����\n");
        printf("10. ��������� ���������\n");
        printf("�������� �����: ");
        scanf("%d", &variant);

        switch (variant) {
        case 1:
            // ����� ��������� ����������
            read_materials_file(fname, &materialCount);

            for (int i = 0; i < materialCount; i++) {
                printf("%d. %s\n", i + 1, materials[i].name);
                printf("����� �� ������ - %1.lfx%1.lf\n������� - %.1lf\n", materials[i].length, materials[i].width, materials[i].square);
            }
            scanf("%d", &marerialls);
            break;

        case 2:
            // ������� ���������� �������
            room_data.S = set_room_parameters(&room_data);
            break;

        case 3:
            // ������� ���������� ������� �������
            room_data.area_of_openings = set_openings_parameters(&room_data);
            break;

        case 4:
            // ������ � ����� �����������
            if (room_data.height > 0 && room_data.length1 > 0 && room_data.length2 > 0) {
                    output_the_result_without_openings(&room_data);

                if (room_data.area_of_openings > room_data.S) {
                    printf("\n�� ����� ������������ ��������, ������� ������ ������ ������� �������.\n ��������� ��������� ������!\n\n");
                    scanf("%d", &variant);
                }
                else {
                   room_data.S_openings = output_of_the_result_with_openings(&room_data);
                   room_data.rolls = number_of_rolls(&room_data);
                    printf("\n����������? ");
                    scanf("%d", &variant);
                    break;
                }
            }
            else {
                printf("\n������: ����������, ������� ������� ��������� �������.\n");
            }

            printf("\n����������?. ����� �����-����������, 10-���������\n");
            scanf("%d", &variant);
            break;

        case 5:
            // ����� ���������� ����
           room_data.glue = output_of_glue_amount(&room_data);
            printf("\n����������?. ����� �����-����������, 10-���������\n");
            scanf("%d", &variant);
            break;

        case 6:
            // ������ ����������� � ����
            if (room_data.height > 0 && room_data.length1 > 0 && room_data.length2 > 0) {
                write_results_to_file(&room_data);
                printf("\n����� �������� � ����� 'results.txt'\n");
            }
            else {
                printf("\n������: ����������, ������� ������� ��������� �������.\n");
            }
            printf("\n����������?. ����� �����-����������, 10-���������\n");
            scanf("%d", &variant);
            break;

        case 10:
            break;
        }

    } while (variant != 10);

    return 0;
}

// ������� ������ ���������� �� �����
struct materialroom* read_materials_file(char* fname, int* materialCount) {
    FILE* file = fopen(fname, "r");
    if (!file) {
        puts("�� ���������� ������� ����");
        return 0;
    }

    struct materialroom* materials = malloc(MAX_MATERIALS * sizeof(struct materialroom));
    if (!materials) {
        puts("�� ���������� �������� ������ ��� ������� ��������");
        return 0;
    }

    int count = 0;
    char ly[1000];

    while (fgets(ly, sizeof(ly), file) != NULL) {
        // ���������� �������� ���������
        for (int i = 0; i < strlen(ly); i++) {
            if (ly[2 + i] == ' ' && ly[2 + i + 1] == ' ') {
                materials[count].name[i] = '\0';
                break;
            }
            materials[count].name[i] = ly[2 + i];
        }

        char buf[1000];

        // ���������� ���������� ���������
        for (int i = 0; i < strlen(ly); i++) {
            buf[i] = ly[30 + i];
        }

        sscanf(buf, "|    | %lfx%lf |    |%lf �^2|", &materials[count].length, &materials[count].width, &materials[count].square);

        count++;
    }

    fclose(file);
    *materialCount = count;
    return materials;
}

// ������� ������� ���������� �������
int set_room_parameters(struct room* room) {
    printf("\n������� ������ �������\n");
    printf("������ ������� = ");
    scanf("%d", &(room->height));

    printf("\n������� ����� ����� ����� �������\n");
    printf("����� ����� ����� ������� = ");
    scanf("%d", &(room->length1));

    printf("\n������� ����� ������ ����� �������\n");
    printf("����� ������ ����� ������� = ");
    scanf("%d", &(room->length2));

    // ������ ������� ���� ������� ��� �������
   return (room->length1 * room->height + room->length2 * room->height) * 2;
}

// ������� ������� ���������� ������� �������
int set_openings_parameters(struct room* room) {
    printf("\n������� ���������� ���� � �������:\n");
    scanf("%d", &(room->window_count));

    printf("\n������� ������� ���� (������ � �����):\n");
    scanf("%d%d", &(room->window_width), &(room->window_length));

    printf("\n������� ���������� ������ � �������:\n");
    scanf("%d", &(room->door_count));

    printf("\n������� ������� ����� (������ � �����):\n");
    scanf("%d%d", &(room->door_width), &(room->door_length));

    return (room->window_count * room->window_width * room->window_length) + (room->door_width * room->door_length * room->door_count);
}

// ������� ������ ������� ���� ��� �������
void output_the_result_without_openings(struct room* room) {
     printf("������� ���� ������� ��� ������ = %d ��^2 (%.2lf �^2)\n", room->S, room->S / 10000.);
}

// ������� ������ ������� ���� � ������ �������
int output_of_the_result_with_openings(struct room* room) {
    room->S_openings = room->S - room->area_of_openings;
    printf("\n������� ���� ���� � ������ ������ = %d ��^2 (%.2lf �^2)\n", room->S_openings, room->S_openings / 10000.);
    return room->S_openings;
}

// ������� ������� ���������� �������
int number_of_rolls(struct room* room) {
    room->rolls = ((room->S - room->area_of_openings) / 220) + 1;
    printf("��� ����������� %d �������, ����� ������� ������� ���������������� ����������", room->rolls);
    return room->rolls;
}

// ������� ������� � ������ ���������� ����
int output_of_glue_amount(struct room* room) {
    room->glue = (room->S - room->area_of_openings) / 2.;
    printf("��� ����������� %d �� ���� (%.2lf ������)\n", room->glue, room->glue / 10000.);
    write_results_to_file(room);
    return room->glue;
}

// ������� ������ ����������� � ����
void write_results_to_file(struct room* room) {
    FILE* file = fopen("results.txt", "w");

    if (file == NULL) {
        printf("������ �������� ����� ��� ������\n");
        return -1;
    }
    else {

        fprintf(file, "������ �������: %d �� (%.3lf �^2)\n", room->height, (room->height) / 10000.);
        fprintf(file, "����� ����� 1: %d �� (%.3lf �^2)\n", room->length1, (room->length1) / 10000.);
        fprintf(file, "����� ����� 2: %d �� (%.3lf �^2)\n", room->length2, (room->length2) / 10000.);
        fprintf(file, "������� ���� ���� ��� ����� ������ = %d ��^2 (%.2lf �^2)\n", room->S, room->S / 10000.);
        fprintf(file, "���������� ����: %d\n", room->window_count);
        fprintf(file, "������� ����: %dx%d ��\n", room->window_width, room->window_length);
        fprintf(file, "���������� ������: %d\n", room->door_count);
        fprintf(file, "������� ������: %dx%d ��\n", room->door_width, room->door_length);
        fprintf(file, "������� ���� ���� � ������ ������ = %d ��^2 (%.2lf �^2)\n", room->S_openings, room->S_openings / 10000.);
        fprintf(file, "��� ����������� %d �������, ����� ������� ������� ���������������� ����������\n", room->rolls);
        fprintf(file, "��� ����������� %d �� ���� (%.2lf ������)\n", room->glue, room->glue / 10000.);

        if (fclose(file) == 0) {
            return 1;
        }
        else {
            printf("������ �������� �����\n");
            return -1;
        }
    }
}
