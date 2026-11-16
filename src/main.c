#include <eadk.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define W 320
#define H 222

#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE  0x001F
#define RED   0xF800
#define GREY  0x7BEF

typedef struct {
    const char *title;
    const char *content;
} Course;

// Données intégrées en dur
static const Course courses[] = {
    {"Electricite - Bases", "Loi d'Ohm : U = R x I\nI = U / R ; R = U / I\nPuissance : P = U x I\nEnergie : E = P x t\n1000 W x 2 h = 2 kWh\nVoltmetre : derivation\nAmperemetre : en serie\nOhmmetre : circuit hors tension"},
    {"MELEC - Securite", "Disjoncteur : surintensites\nDifferentiel : desequilibre (fuite)\nFusible : element qui fond\nRespecter normes, EPI et consignes\nPas d'intervention sans autorisation"},
    {"Composants & Reseaux", "Transfo : U2/U1 = N2/N1\nCondensateur : Q = C x U\nMoteur : Elec -> Mecanique\nAlternateur : Meca -> Elec\nContinu : sens constant\nAlternatif : evolue periodiquement"},
    {"Mathematiques", "Pythagore : a2 + b2 = c2\nAffine : f(x) = ax + b\nLineaire : f(x) = ax\nPourcentage : x % de A = A * x / 100\nMoyenne = somme / effectif\nEtendue = max - min"},
    {"Francais", "Metaphore : sans outil\nComparaison : outil (comme)\nPersonnification : humain\nHyperbole : exageration"},
    {"Anglais", "Preterit : action terminee dans le passe\nReguliers : base + ed\nIrreguliers : forme propre\nExemple : I visited London"},
    {"Histoire - Geo", "1789 : fin de monarchie absolue\nRevolution francaise\nChangement climatique : causes humaines\nMigrations climatiques et conflits"}
};

#define COURSES_COUNT (sizeof(courses) / sizeof(courses[0]))

static void clear_screen(void) {
    eadk_display_push_rect_uniform((eadk_rect_t){0, 0, W, H}, WHITE);
}

// Fonction pour afficher du texte multiligne proprement
static void draw_text(const char *text, int start_y) {
    char line[50];
    int line_idx = 0;
    int y = start_y;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n' || line_idx == 45) {
            line[line_idx] = '\0';
            eadk_display_draw_string(line, (eadk_point_t){5, y}, false, BLACK, WHITE);
            y += 18;
            line_idx = 0;
            if (text[i] == '\n') continue;
        }
        line[line_idx++] = text[i];
    }
    if (line_idx > 0) {
        line[line_idx] = '\0';
        eadk_display_draw_string(line, (eadk_point_t){5, y}, false, BLACK, WHITE);
    }
}

static void show_course(int index) {
    clear_screen();
    
    // En-tête
    eadk_display_push_rect_uniform((eadk_rect_t){0, 0, W, 24}, RED);
    eadk_display_draw_string(courses[index].title, (eadk_point_t){5, 5}, false, WHITE, RED);

    // Contenu
    draw_text(courses[index].content, 35);

    // Pied de page
    eadk_display_push_rect_uniform((eadk_rect_t){0, 200, W, 22}, GREY);
    eadk_display_draw_string("BACK : Retour a la liste", (eadk_point_t){5, 203}, false, BLACK, GREY);

    // Attente de la touche retour
    while (1) {
        eadk_keyboard_state_t keys = eadk_keyboard_scan();
        if (keys & (1ULL << EADK_KEY_BACK)) {
            break;
        }
        eadk_timing_msleep(50);
    }
}

int main(void) {
    int selected = 0;
    bool redraw = true;

    while (1) {
        if (redraw) {
            clear_screen();
            
            // Titre du menu
            eadk_display_push_rect_uniform((eadk_rect_t){0, 0, W, 24}, BLUE);
            eadk_display_draw_string("AELYX V3 - COURS HORS LIGNE", (eadk_point_t){5, 5}, false, WHITE, BLUE);

            // Affichage de la liste des cours
            for (unsigned int i = 0; i < COURSES_COUNT; i++) {
                eadk_color_t bg = (i == selected) ? BLUE : WHITE;
                eadk_color_t fg = (i == selected) ? WHITE : BLACK;
                
                eadk_display_push_rect_uniform((eadk_rect_t){0, 30 + i * 20, W, 20}, bg);
                eadk_display_draw_string(courses[i].title, (eadk_point_t){10, 32 + i * 20}, false, fg, bg);
            }

            // Pied de page
            eadk_display_push_rect_uniform((eadk_rect_t){0, 200, W, 22}, GREY);
            eadk_display_draw_string("HAUT/BAS : Choisir   OK : Ouvrir", (eadk_point_t){5, 203}, false, BLACK, GREY);
            
            redraw = false;
        }

        // Gestion des touches
        eadk_keyboard_state_t keys = eadk_keyboard_scan();

        if (keys & (1ULL << EADK_KEY_UP)) {
            if (selected > 0) {
                selected--;
                redraw = true;
            }
            eadk_timing_msleep(150); // Anti-rebond
        }
        
        if (keys & (1ULL << EADK_KEY_DOWN)) {
            if (selected < COURSES_COUNT - 1) {
                selected++;
                redraw = true;
            }
            eadk_timing_msleep(150);
        }
        
        if (keys & (1ULL << EADK_KEY_OK)) {
            show_course(selected);
            redraw = true;
            eadk_timing_msleep(150);
        }

        eadk_timing_msleep(50);
    }
    return 0;
}
