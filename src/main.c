#include <eadk.h>
#include <stdbool.h>
#include <stdint.h>

#define W 320
#define H 222

#define C_WHITE 0xFFFF
#define C_BLACK 0x0000
#define C_BLUE  0x001F
#define C_RED   0xF800
#define C_GREY  0x7BEF

typedef struct {
    const char *title;
    const char *content;
} Course;

static const Course courses[] = {
    {"Electricite - Bases", "Loi d'Ohm : U = R x I\nI = U / R ; R = U / I\nPuissance : P = U x I\nEnergie : E = P x t\n1000 W x 2 h = 2 kWh\nVoltmetre : derivation\nAmperemetre : en serie\nOhmmetre : hors tension"},
    {"MELEC - Securite", "Disjoncteur : surintensites\nDifferentiel : desequilibre (fuite)\nFusible : element qui fond\nRespecter normes, EPI, consignes\nPas d'intervention sans autorisation"},
    {"Composants & Reseaux", "Transfo : U2/U1 = N2/N1\nCondensateur : Q = C x U\nMoteur : Elec -> Mecanique\nAlternateur : Meca -> Elec\nContinu : sens constant\nAlternatif : evolue periodiquement"},
    {"Mathematiques", "Pythagore : a2 + b2 = c2\nAffine : f(x) = ax + b\nLineaire : f(x) = ax\nPourcentage : x% de A = A * x / 100\nMoyenne = somme / effectif\nEtendue = max - min"},
    {"Francais", "Metaphore : sans outil\nComparaison : outil (comme)\nPersonnification : humain\nHyperbole : exageration"},
    {"Anglais", "Preterit : action terminee dans le passe\nReguliers : base + ed\nIrreguliers : forme propre\nExemple : I visited London"},
    {"Histoire - Geo", "1789 : fin de monarchie absolue\nRevolution francaise\nChangement climatique : causes humaines\nMigrations climatiques et conflits"}
};

#define COURSES_COUNT (sizeof(courses) / sizeof(courses[0]))

static void clear_screen(void) {
    eadk_rect_t rect = {0, 0, W, H};
    eadk_display_push_rect_uniform(rect, C_WHITE);
}

static void draw_text(const char *text, uint16_t start_y) {
    char line[50];
    int line_idx = 0;
    uint16_t y = start_y;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n' || line_idx == 45) {
            line[line_idx] = '\0';
            eadk_point_t pt = {5, y};
            eadk_display_draw_string(line, pt, false, C_BLACK, C_WHITE);
            y += 18;
            line_idx = 0;
            if (text[i] == '\n') continue;
        }
        line[line_idx++] = text[i];
    }
    if (line_idx > 0) {
        line[line_idx] = '\0';
        eadk_point_t pt = {5, y};
        eadk_display_draw_string(line, pt, false, C_BLACK, C_WHITE);
    }
}

static void show_course(int index) {
    clear_screen();
    
    eadk_rect_t header_rect = {0, 0, W, 24};
    eadk_display_push_rect_uniform(header_rect, C_RED);
    
    eadk_point_t header_pt = {5, 5};
    eadk_display_draw_string(courses[index].title, header_pt, false, C_WHITE, C_RED);

    draw_text(courses[index].content, 35);

    eadk_rect_t footer_rect = {0, 200, W, 22};
    eadk_display_push_rect_uniform(footer_rect, C_GREY);
    
    eadk_point_t footer_pt = {5, 203};
    eadk_display_draw_string("BACK : Retour a la liste", footer_pt, false, C_BLACK, C_GREY);

    while (1) {
        eadk_keyboard_state_t keys = eadk_keyboard_scan();
        if (keys & (1ULL << eadk_key_back)) {
            break;
        }
        eadk_timing_msleep(50);
    }
}

int main(int argc, const char * argv[]) {
    (void)argc;
    (void)argv;
    
    int selected = 0;
    bool redraw = true;

    while (1) {
        if (redraw) {
            clear_screen();
            
            eadk_rect_t title_rect = {0, 0, W, 24};
            eadk_display_push_rect_uniform(title_rect, C_BLUE);
            
            eadk_point_t title_pt = {5, 5};
            eadk_display_draw_string("AELYX V3 - COURS HORS LIGNE", title_pt, false, C_WHITE, C_BLUE);

            for (unsigned int i = 0; i < COURSES_COUNT; i++) {
                eadk_color_t bg = (i == selected) ? C_BLUE : C_WHITE;
                eadk_color_t fg = (i == selected) ? C_WHITE : C_BLACK;
                
                eadk_rect_t row_rect = {0, (uint16_t)(30 + i * 20), W, 20};
                eadk_display_push_rect_uniform(row_rect, bg);
                
                eadk_point_t row_pt = {10, (uint16_t)(32 + i * 20)};
                eadk_display_draw_string(courses[i].title, row_pt, false, fg, bg);
            }

            eadk_rect_t footer_rect = {0, 200, W, 22};
            eadk_display_push_rect_uniform(footer_rect, C_GREY);
            
            eadk_point_t footer_pt = {5, 203};
            eadk_display_draw_string("HAUT/BAS : Choisir   OK : Ouvrir", footer_pt, false, C_BLACK, C_GREY);
            
            redraw = false;
        }

        eadk_keyboard_state_t keys = eadk_keyboard_scan();

        if (keys & (1ULL << eadk_key_up)) {
            if (selected > 0) {
                selected--;
                redraw = true;
            }
            eadk_timing_msleep(150);
        }
        
        if (keys & (1ULL << eadk_key_down)) {
            if (selected < COURSES_COUNT - 1) {
                selected++;
                redraw = true;
            }
            eadk_timing_msleep(150);
        }
        
        if (keys & (1ULL << eadk_key_ok)) {
            show_course(selected);
            redraw = true;
            eadk_timing_msleep(150);
        }

        eadk_timing_msleep(50);
    }
    return 0;
}
