/**
 * @file lv_demo_widgets.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_demo_widgets.h"

#if LV_USE_DEMO_WIDGETS

#if LV_MEM_CUSTOM == 0 && LV_MEM_SIZE < (38ul * 1024ul)
    #error Insufficient memory for lv_demo_widgets. Please set LV_MEM_SIZE to at least 38KB (38ul * 1024ul).  48KB is recommended.
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    DISP_SMALL,
    DISP_MEDIUM,
    DISP_LARGE,
}disp_size_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void profile_create(lv_obj_t * parent);
static void analytics_create(lv_obj_t * parent);
static void shop_create(lv_obj_t * parent);
static void color_changer_create(lv_obj_t * parent);

static lv_obj_t * create_meter_box(lv_obj_t * parent, const char * title, const char * text1, const char * text2, const char * text3);
static lv_obj_t * create_shop_item(lv_obj_t * parent, const void * img_src, const char * name, const char * category, const char * price);

static void color_changer_event_cb(lv_event_t * e);
static void color_event_cb(lv_event_t * e);
static void ta_event_cb(lv_event_t * e);
static void birthday_event_cb(lv_event_t * e);
static void calendar_event_cb(lv_event_t * e);
static void slider_event_cb(lv_event_t * e);
static void chart_event_cb(lv_event_t * e);
static void shop_chart_event_cb(lv_event_t * e);

static void chart2_ser2_bar0_anim_cb(void * var, int32_t v);
static void chart2_ser2_bar1_anim_cb(void * var, int32_t v);
static void chart2_ser2_bar2_anim_cb(void * var, int32_t v);
static void chart2_ser2_bar3_anim_cb(void * var, int32_t v);
static void chart2_ser3_bar0_anim_cb(void * var, int32_t v);
static void chart2_ser3_bar1_anim_cb(void * var, int32_t v);
static void chart2_ser3_bar2_anim_cb(void * var, int32_t v);
static void chart2_ser3_bar3_anim_cb(void * var, int32_t v);
static void tableview_switch_event(lv_event_t * e);

static void meter1_indic1_anim_cb(void * var, int32_t v);
static void meter1_indic2_anim_cb(void * var, int32_t v);
static void meter1_indic3_anim_cb(void * var, int32_t v);
static void meter2_timer_cb(lv_timer_t * timer);
static void meter3_anim_cb(void * var, int32_t v);

/**********************
 *  STATIC VARIABLES
 **********************/
static disp_size_t disp_size;

static lv_obj_t * tv;
static lv_obj_t * calendar;
static lv_style_t style_text_muted;
static lv_style_t style_title;
static lv_style_t style_icon;
static lv_style_t style_bullet;

static lv_obj_t * meter1;
static lv_obj_t * meter2;
static lv_obj_t * meter3;

static lv_obj_t * chart1;
static lv_obj_t * chart2;
static lv_obj_t * chart3;

static lv_chart_series_t * ser1;
static lv_chart_series_t * ser2;
static lv_chart_series_t * ser3;
static lv_chart_series_t * ser4;

static const lv_font_t * font_large;
static const lv_font_t * font_normal;

static uint32_t session_desktop = 1000;
static uint32_t session_tablet = 1000;
static uint32_t session_mobile = 1000;


extern volatile uint32_t g_msModelCalTime[8];

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_demo_widgets(void)
{
    if(LV_HOR_RES <= 320) disp_size = DISP_SMALL;
    else if(LV_HOR_RES < 720) disp_size = DISP_MEDIUM;
    else disp_size = DISP_LARGE;

    font_large = LV_FONT_DEFAULT;
    font_normal = LV_FONT_DEFAULT;

    lv_coord_t tab_h;
    if(disp_size == DISP_LARGE) {
        tab_h = 70;
#if LV_FONT_MONTSERRAT_24
        font_large     = &lv_font_montserrat_24;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_24 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
#if LV_FONT_MONTSERRAT_16
        font_normal    = &lv_font_montserrat_16;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_16 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
    } else if(disp_size == DISP_MEDIUM) {
        tab_h = 45;
#if LV_FONT_MONTSERRAT_20
        font_large     = &lv_font_montserrat_20;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_20 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
#if LV_FONT_MONTSERRAT_14
        font_normal    = &lv_font_montserrat_14;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_14 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
    } else { /* disp_size == DISP_SMALL */
        tab_h = 45;
#if LV_FONT_MONTSERRAT_18
        font_large     = &lv_font_montserrat_18;
#else
    LV_LOG_WARN("LV_FONT_MONTSERRAT_18 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
#if LV_FONT_MONTSERRAT_12
        font_normal    = &lv_font_montserrat_12;
#else
    LV_LOG_WARN("LV_FONT_MONTSERRAT_12 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
    }

#if LV_USE_THEME_DEFAULT
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, font_normal);
#endif
    font_large     = &lv_font_montserrat_16;

    lv_style_init(&style_text_muted);
    lv_style_set_text_opa(&style_text_muted, LV_OPA_50);
    lv_style_set_text_font(&style_text_muted, font_large);

    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, font_large);

    lv_style_init(&style_icon);
    lv_style_set_text_color(&style_icon, lv_theme_get_color_primary(NULL));
    lv_style_set_text_font(&style_icon, font_large);

    lv_style_init(&style_bullet);
    lv_style_set_border_width(&style_bullet, 0);
    lv_style_set_radius(&style_bullet, LV_RADIUS_CIRCLE);

    tv = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, tab_h);
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);

    if(disp_size == DISP_LARGE) {
        lv_obj_t * tab_btns = lv_tabview_get_tab_btns(tv);
        lv_obj_set_style_pad_left(tab_btns, LV_HOR_RES / 2, 0);
//        lv_obj_t * logo = lv_img_create(tab_btns);
//        LV_IMG_DECLARE(NXP_LOGO);
//        lv_img_set_src(logo, &NXP_LOGO);
//        lv_obj_align(logo, LV_ALIGN_LEFT_MID, -LV_HOR_RES / 2 + 25, 0);

        lv_obj_t * label = lv_label_create(tab_btns);
        lv_obj_add_style(label, &style_title, 0);
        lv_label_set_text(label, "MCXN9xx");
//        lv_obj_align_to(label, logo, LV_ALIGN_OUT_RIGHT_TOP, 10, 0);
        lv_obj_set_pos(label, -LV_HOR_RES / 2 + 10, 0);

        label = lv_label_create(tab_btns);
        lv_label_set_text(label, "AI Performance Demo");
        lv_obj_add_style(label, &style_text_muted, 0);
//        lv_obj_align_to(label, logo, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, 10);
        lv_obj_set_pos(label, -LV_HOR_RES / 2 + 10, 20);
    }

    lv_obj_t * t1 = lv_tabview_add_tab(tv, "Introduction");
    lv_obj_t * t2 = lv_tabview_add_tab(tv, "Performance");
    lv_obj_add_event_cb(tv, tableview_switch_event, LV_EVENT_VALUE_CHANGED, NULL);


    profile_create(t1);
    analytics_create(t2);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void profile_create(lv_obj_t * parent)
{

    lv_obj_t * panel1 = lv_obj_create(parent);
    lv_obj_set_size(panel1, 760, 360);
    lv_obj_set_scrollbar_mode(panel1, LV_SCROLLBAR_MODE_OFF);

//    lv_obj_t * avatar = lv_img_create(panel1);
//    LV_IMG_DECLARE(motor);
//    lv_img_set_src(avatar, &motor);
//    lv_obj_set_pos(avatar,0,0);

    lv_obj_t * label_motor = lv_label_create(panel1);
    lv_obj_set_pos(label_motor,135,30);
    lv_obj_set_style_text_font(label_motor, font_large, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_motor,lv_color_make(0xff, 0x0, 0x00), LV_PART_MAIN);
    lv_label_set_text(label_motor, "Anomaly Detection (AD)");
    lv_obj_t * label_motor_detail = lv_label_create(panel1);
    lv_obj_set_pos(label_motor_detail,135,60);
    lv_obj_set_style_text_font(label_motor_detail, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_motor_detail,lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN);
    lv_label_set_text(label_motor_detail, "Which uses a neural network\nto identify abnormalities in\nmachine operating sounds.");

//    lv_obj_t * avatar1 = lv_img_create(panel1);
//    LV_IMG_DECLARE(kws);
//    lv_img_set_src(avatar1, &kws);
//    lv_obj_set_pos(avatar1,370,0);

    lv_obj_t * label_kws = lv_label_create(panel1);
    lv_obj_set_pos(label_kws,505,30);
    lv_obj_set_style_text_font(label_kws, font_large, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_kws,lv_color_make(0xff, 0x0, 0x00), LV_PART_MAIN);
    lv_label_set_text(label_kws, "Keyword Spotting (KWS)");
    lv_obj_t * label_kws_detail = lv_label_create(panel1);
    lv_obj_set_pos(label_kws_detail,505,60);
    lv_obj_set_style_text_font(label_kws_detail, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_kws_detail,lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN);
    lv_label_set_text(label_kws_detail, "Which uses a neural network\nthat detects keywords from\na spectrogram");

//    lv_obj_t * avatar2 = lv_img_create(panel1);
//    LV_IMG_DECLARE(picture_classfication);
//    lv_img_set_src(avatar2, &picture_classfication);
//    lv_obj_set_pos(avatar2,0,170);

    lv_obj_t * label_picture = lv_label_create(panel1);
    lv_obj_set_pos(label_picture,135,170+30);
    lv_obj_set_style_text_font(label_picture, font_large, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_picture,lv_color_make(0xff, 0x0, 0x00), LV_PART_MAIN);
    lv_label_set_text(label_picture, "Tiny Image Classification (IC)");
    lv_obj_t * label_picture_detail = lv_label_create(panel1);
    lv_obj_set_pos(label_picture_detail,135,170+60);
    lv_obj_set_style_text_font(label_picture_detail, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_picture_detail,lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN);
    lv_label_set_text(label_picture_detail, "A small image classification \nbenchmark with 10 classes;");

//    lv_obj_t * avatar3 = lv_img_create(panel1);
//    LV_IMG_DECLARE(virsual_wake_word);
//    lv_img_set_src(avatar3, &virsual_wake_word);
//    lv_obj_set_pos(avatar3,370,170);

    lv_obj_t * label_virsual = lv_label_create(panel1);
    lv_obj_set_pos(label_virsual,505,170+30);
    lv_obj_set_style_text_font(label_virsual, font_large, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_virsual,lv_color_make(0xff, 0x0, 0x00), LV_PART_MAIN);
    lv_label_set_text(label_virsual, "Visual Wake Words (VWW)");
    lv_obj_t * label_virsual_detail = lv_label_create(panel1);
    lv_obj_set_pos(label_virsual_detail,505,170+60);
    lv_obj_set_style_text_font(label_virsual_detail, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_virsual_detail,lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN);
    lv_label_set_text(label_virsual_detail, "A binary image classification task\nfor determining the presence of a\nperson in an image");

}


static void analytics_create(lv_obj_t * parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_ROW_WRAP);

    static lv_coord_t grid_chart_row_dsc[] = {LV_GRID_CONTENT, LV_GRID_FR(1), 10, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t grid_chart_col_dsc[] = {20, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_t * chart2_cont = lv_obj_create(parent);
    lv_obj_add_flag(chart2_cont, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    lv_obj_set_flex_grow(chart2_cont, 1);

    lv_obj_set_height(chart2_cont, LV_PCT(200));
    lv_obj_set_style_max_height(chart2_cont, 380, 0);

    lv_obj_set_grid_dsc_array(chart2_cont, grid_chart_col_dsc, grid_chart_row_dsc);

    lv_obj_t * title = lv_label_create(chart2_cont);
    lv_label_set_text(title, "AI Performance");
    lv_obj_add_style(title, &style_title, 0);
    lv_obj_set_grid_cell(title, LV_GRID_ALIGN_START, 0, 2, LV_GRID_ALIGN_START, 0, 1);

    chart2 = lv_chart_create(chart2_cont);
    lv_group_add_obj(lv_group_get_default(), chart2);
    lv_obj_add_flag(chart2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    lv_obj_set_grid_cell(chart2, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_chart_set_axis_tick(chart2, LV_CHART_AXIS_PRIMARY_Y, 0, 0, 6, 1, true, 80);
    lv_chart_set_axis_tick(chart2, LV_CHART_AXIS_PRIMARY_X, 0, 0, 4, 1, true, 80);
    lv_obj_set_size(chart2, LV_PCT(100), LV_PCT(100));
    lv_chart_set_range(chart2,LV_CHART_AXIS_PRIMARY_Y, 0, 500);
    lv_chart_set_type(chart2, LV_CHART_TYPE_BAR);
    lv_chart_set_div_line_count(chart2, 6, 0);
    lv_chart_set_point_count(chart2, 4);
    lv_obj_add_event_cb(chart2, chart_event_cb, LV_EVENT_ALL, NULL);
    lv_chart_set_zoom_x(chart2, 128 * 2);
    lv_obj_set_style_border_side(chart2, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_radius(chart2, 0, 0);

    if(disp_size == DISP_SMALL) {
        lv_obj_set_style_pad_gap(chart2, 0, LV_PART_ITEMS);
        lv_obj_set_style_pad_gap(chart2, 2, LV_PART_MAIN);
    }
    else if(disp_size == DISP_LARGE) {
        lv_obj_set_style_pad_gap(chart2, 16, 0);
    }

    ser2 = lv_chart_add_series(chart2, lv_palette_lighten(LV_PALETTE_GREEN, 1), LV_CHART_AXIS_PRIMARY_Y);

    ser3 = lv_chart_add_series(chart2, lv_theme_get_color_primary(chart1), LV_CHART_AXIS_PRIMARY_Y);

    lv_obj_t * title_model1 = lv_label_create(chart2);
    lv_label_set_text(title_model1, "AD01_Int8");
    lv_obj_add_style(title_model1, &style_title, 0);
    lv_obj_set_pos(title_model1, 40,0);

    lv_obj_t * title_model2 = lv_label_create(chart2);
    lv_label_set_text(title_model2, "KWS_Int8");
    lv_obj_add_style(title_model2, &style_title, 0);
    lv_obj_set_pos(title_model2, 160+40,0);


    lv_obj_t * title_model3 = lv_label_create(chart2);
    lv_label_set_text(title_model3, "ResNet_Int8");
    lv_obj_add_style(title_model3, &style_title, 0);
    lv_obj_set_pos(title_model3, 160*2+40,0);

    lv_obj_t * title_model4 = lv_label_create(chart2);
    lv_label_set_text(title_model4, "VWW_Int8");
    lv_obj_add_style(title_model4, &style_title, 0);
    lv_obj_set_pos(title_model4, 160*3+60,0);



 }

static void color_changer_create(lv_obj_t * parent)
{
    static lv_palette_t palette[] = {
            LV_PALETTE_BLUE, LV_PALETTE_GREEN, LV_PALETTE_BLUE_GREY,  LV_PALETTE_ORANGE,
            LV_PALETTE_RED, LV_PALETTE_PURPLE, LV_PALETTE_TEAL, _LV_PALETTE_LAST };

    lv_obj_t * color_cont = lv_obj_create(parent);
    lv_obj_remove_style_all(color_cont);
    lv_obj_set_flex_flow(color_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(color_cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_flag(color_cont, LV_OBJ_FLAG_FLOATING);

    lv_obj_set_style_bg_color(color_cont, lv_color_white(), 0);
    lv_obj_set_style_pad_right(color_cont, disp_size == DISP_SMALL ? LV_DPX(47) : LV_DPX(55), 0);
    lv_obj_set_style_bg_opa(color_cont, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(color_cont, LV_RADIUS_CIRCLE, 0);

    if(disp_size == DISP_SMALL) lv_obj_set_size(color_cont, LV_DPX(52), LV_DPX(52));
    else lv_obj_set_size(color_cont, LV_DPX(60), LV_DPX(60));

    lv_obj_align(color_cont, LV_ALIGN_BOTTOM_RIGHT, - LV_DPX(10),  - LV_DPX(10));

    uint32_t i;
    for(i = 0; palette[i] != _LV_PALETTE_LAST; i++) {
        lv_obj_t * c = lv_btn_create(color_cont);
        lv_obj_set_style_bg_color(c, lv_palette_main(palette[i]), 0);
        lv_obj_set_style_radius(c, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_opa(c, LV_OPA_TRANSP, 0);
        lv_obj_set_size(c, 20, 20);
        lv_obj_add_event_cb(c, color_event_cb, LV_EVENT_ALL, &palette[i]);
        lv_obj_clear_flag(c, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    }

    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_FLOATING | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(btn, lv_color_white(), LV_STATE_CHECKED);
    lv_obj_set_style_pad_all(btn, 10, 0);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_add_event_cb(btn, color_changer_event_cb, LV_EVENT_ALL, color_cont);
    lv_obj_set_style_shadow_width(btn, 0, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_TINT, 0);

    if(disp_size == DISP_SMALL) {
         lv_obj_set_size(btn, LV_DPX(42), LV_DPX(42));
         lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, -LV_DPX(15), -LV_DPX(15));
     } else {
         lv_obj_set_size(btn, LV_DPX(50), LV_DPX(50));
         lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, -LV_DPX(15), -LV_DPX(15));
     }
}

static void color_changer_anim_cb(void * var, int32_t v)
{
    lv_obj_t * obj = var;
    lv_coord_t max_w = lv_obj_get_width(lv_obj_get_parent(obj)) - LV_DPX(20);
    lv_coord_t w;

    if(disp_size == DISP_SMALL) {
        w = lv_map(v, 0, 256, LV_DPX(52), max_w);
        lv_obj_set_width(obj, w);
        lv_obj_align(obj, LV_ALIGN_BOTTOM_RIGHT, - LV_DPX(10),  - LV_DPX(10));
    } else {
        w = lv_map(v, 0, 256, LV_DPX(60), max_w);
        lv_obj_set_width(obj, w);
        lv_obj_align(obj, LV_ALIGN_BOTTOM_RIGHT, - LV_DPX(10),  - LV_DPX(10));
    }

    if(v > LV_OPA_COVER) v = LV_OPA_COVER;

    uint32_t i;
    for(i = 0; i < lv_obj_get_child_cnt(obj); i++) {
        lv_obj_set_style_opa(lv_obj_get_child(obj, i), v, 0);
    }

}

static void color_changer_event_cb(lv_event_t *e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_obj_t * color_cont = lv_event_get_user_data(e);
        if(lv_obj_get_width(color_cont) < LV_HOR_RES / 2) {
            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_var(&a, color_cont);
            lv_anim_set_exec_cb(&a, color_changer_anim_cb);
            lv_anim_set_values(&a, 0, 256);
            lv_anim_set_time(&a, 200);
            lv_anim_start(&a);
        } else {
            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_var(&a, color_cont);
            lv_anim_set_exec_cb(&a, color_changer_anim_cb);
            lv_anim_set_values(&a, 256, 0);
            lv_anim_set_time(&a, 200);
            lv_anim_start(&a);
        }
    }
}
static void color_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_FOCUSED) {
        lv_obj_t * color_cont = lv_obj_get_parent(obj);
        if(lv_obj_get_width(color_cont) < LV_HOR_RES / 2) {
            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_var(&a, color_cont);
            lv_anim_set_exec_cb(&a, color_changer_anim_cb);
            lv_anim_set_values(&a, 0, 256);
            lv_anim_set_time(&a, 200);
            lv_anim_start(&a);
        }
    }
    else if(code == LV_EVENT_CLICKED) {
        lv_palette_t * palette_primary = lv_event_get_user_data(e);
        lv_palette_t palette_secondary = (*palette_primary) + 3; /*Use another palette as secondary*/
        if(palette_secondary >= _LV_PALETTE_LAST) palette_secondary = 0;

        lv_theme_default_init(NULL, lv_palette_main(*palette_primary), lv_palette_main(palette_secondary), LV_THEME_DEFAULT_DARK, font_normal);

        lv_color_t color = lv_palette_main(*palette_primary);
        lv_style_set_text_color(&style_icon, color);
        lv_chart_set_series_color(chart1, ser1, color);
        lv_chart_set_series_color(chart2, ser3, color);
    }
}

static void ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);
    if(code == LV_EVENT_FOCUSED) {
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD) {
            lv_keyboard_set_textarea(kb, ta);
            lv_obj_set_style_max_height(kb, LV_HOR_RES * 2 / 3, 0);
            lv_obj_update_layout(tv);   /*Be sure the sizes are recalculated*/
            lv_obj_set_height(tv, LV_VER_RES - lv_obj_get_height(kb));
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
            lv_obj_scroll_to_view_recursive(ta, LV_ANIM_OFF);
        }
    }
    else if(code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_set_height(tv, LV_VER_RES);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, ta);

    }
    else if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_set_height(tv, LV_VER_RES);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
        lv_indev_reset(NULL, ta);   /*To forget the last clicked object to make it focusable again*/
    }
}

static void tableview_switch_event(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
      if(lv_tabview_get_tab_act(lv_event_get_current_target(e)) == 1){
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_time(&a, 1000);

        lv_anim_set_exec_cb(&a, chart2_ser2_bar0_anim_cb);
        lv_anim_set_values(&a, 0, g_msModelCalTime[4]);
        lv_anim_start(&a);

        lv_anim_set_exec_cb(&a, chart2_ser2_bar1_anim_cb);
        lv_anim_set_values(&a, 0, g_msModelCalTime[5]);
        lv_anim_start(&a);

        lv_anim_set_exec_cb(&a, chart2_ser2_bar2_anim_cb);
        lv_anim_set_values(&a, 0, g_msModelCalTime[6]);
        lv_anim_start(&a);

        lv_anim_set_exec_cb(&a, chart2_ser2_bar3_anim_cb);
        lv_anim_set_values(&a, 0, g_msModelCalTime[7]);
        lv_anim_start(&a);

        lv_anim_set_exec_cb(&a, chart2_ser3_bar0_anim_cb);
        lv_anim_set_values(&a, 0, g_msModelCalTime[0]);
        lv_anim_start(&a);

        lv_anim_set_exec_cb(&a, chart2_ser3_bar1_anim_cb);
        lv_anim_set_values(&a, 0, g_msModelCalTime[1]);
        lv_anim_start(&a);

        lv_anim_set_exec_cb(&a, chart2_ser3_bar2_anim_cb);
        lv_anim_set_values(&a, 0, g_msModelCalTime[2]);
        lv_anim_start(&a);

        lv_anim_set_exec_cb(&a, chart2_ser3_bar3_anim_cb);
        lv_anim_set_values(&a, 0, g_msModelCalTime[3]);
        lv_anim_start(&a);
      }
    }
}

static void chart_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_PRESSED || code == LV_EVENT_RELEASED) {
        lv_obj_invalidate(obj); /*To make the value boxes visible*/
    }
    else if(code == LV_EVENT_DRAW_PART_BEGIN) {
        lv_obj_draw_part_dsc_t * dsc = lv_event_get_param(e);
        /*Set the markers' text*/
        if(dsc->part == LV_PART_TICKS && dsc->id == LV_CHART_AXIS_PRIMARY_X) {
            if(lv_chart_get_type(obj) == LV_CHART_TYPE_LINE) {
                const char * month[] = {"I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X", "XI", "XII"};
                lv_snprintf(dsc->text, dsc->text_length, "%s", month[dsc->value]);
            } else {
                const char * month[] = {"neutron    cm33", "neutron    cm33", "neutron    cm33", "neutron    cm33"};
                lv_snprintf(dsc->text, dsc->text_length, "%s", month[dsc->value]);
            }
        }

        /*Add the faded area before the lines are drawn */
        else if(dsc->part == LV_PART_ITEMS) {
#if LV_DRAW_COMPLEX
            /*Add  a line mask that keeps the area below the line*/
            if(dsc->p1 && dsc->p2) {
                lv_draw_mask_line_param_t line_mask_param;
                lv_draw_mask_line_points_init(&line_mask_param, dsc->p1->x, dsc->p1->y, dsc->p2->x, dsc->p2->y, LV_DRAW_MASK_LINE_SIDE_BOTTOM);
                int16_t line_mask_id = lv_draw_mask_add(&line_mask_param, NULL);

                /*Add a fade effect: transparent bottom covering top*/
                lv_coord_t h = lv_obj_get_height(obj);
                lv_draw_mask_fade_param_t fade_mask_param;
                lv_draw_mask_fade_init(&fade_mask_param, &obj->coords, LV_OPA_COVER, obj->coords.y1 + h / 8, LV_OPA_TRANSP, obj->coords.y2);
                int16_t fade_mask_id = lv_draw_mask_add(&fade_mask_param, NULL);

                /*Draw a rectangle that will be affected by the mask*/
                lv_draw_rect_dsc_t draw_rect_dsc;
                lv_draw_rect_dsc_init(&draw_rect_dsc);
                draw_rect_dsc.bg_opa = LV_OPA_50;
                draw_rect_dsc.bg_color = dsc->line_dsc->color;

                lv_area_t obj_clip_area;
                _lv_area_intersect(&obj_clip_area, dsc->draw_ctx->clip_area, &obj->coords);
                const lv_area_t * clip_area_ori = dsc->draw_ctx->clip_area;
                dsc->draw_ctx->clip_area = &obj_clip_area;
                lv_area_t a;
                a.x1 = dsc->p1->x;
                a.x2 = dsc->p2->x - 1;
                a.y1 = LV_MIN(dsc->p1->y, dsc->p2->y);
                a.y2 = obj->coords.y2;
                lv_draw_rect(dsc->draw_ctx, &draw_rect_dsc, &a);
                dsc->draw_ctx->clip_area = clip_area_ori;
                /*Remove the masks*/
                lv_draw_mask_remove_id(line_mask_id);
                lv_draw_mask_remove_id(fade_mask_id);
            }
#endif

            const lv_chart_series_t * ser = dsc->sub_part_ptr;

            char buf[7];
            lv_snprintf(buf, sizeof(buf), "%"LV_PRIu32" ms", dsc->value);

            lv_point_t text_size;
            lv_txt_get_size(&text_size, buf, font_normal, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);

            lv_area_t txt_area;
            if(lv_chart_get_type(obj) == LV_CHART_TYPE_BAR) {
                txt_area.y2 = dsc->draw_area->y1 - LV_DPX(15);
                txt_area.y1 = txt_area.y2 - text_size.y;
                if(ser == lv_chart_get_series_next(obj, NULL)) {
                    txt_area.x1 = dsc->draw_area->x1 +  LV_DPX(25);
                    txt_area.x2 = txt_area.x1 + text_size.x;
                }
                else {
                    txt_area.x2 = dsc->draw_area->x1 +  LV_DPX(60);;
                    txt_area.x1 = txt_area.x2 - text_size.x -  LV_DPX(3);;
                }
            }
            lv_draw_label_dsc_t label_dsc;
            lv_draw_label_dsc_init(&label_dsc);
            label_dsc.color = lv_color_black();
            label_dsc.font = font_normal;
            lv_draw_label(dsc->draw_ctx, &label_dsc, &txt_area,  buf, NULL);
        }
    }
}


static void chart2_ser2_bar0_anim_cb(void * var, int32_t v){
    lv_chart_set_value_by_id(chart2, ser2, 0, v);
}
static void chart2_ser2_bar1_anim_cb(void * var, int32_t v){
    lv_chart_set_value_by_id(chart2, ser2, 1, v);
}
static void chart2_ser2_bar2_anim_cb(void * var, int32_t v){
    lv_chart_set_value_by_id(chart2, ser2, 2, v);
}
static void chart2_ser2_bar3_anim_cb(void * var, int32_t v){
    lv_chart_set_value_by_id(chart2, ser2, 3, v);
}

static void chart2_ser3_bar0_anim_cb(void * var, int32_t v){
    lv_chart_set_value_by_id(chart2, ser3, 0, v);
}
static void chart2_ser3_bar1_anim_cb(void * var, int32_t v){
    lv_chart_set_value_by_id(chart2, ser3, 1, v);
}
static void chart2_ser3_bar2_anim_cb(void * var, int32_t v){
    lv_chart_set_value_by_id(chart2, ser3, 2, v);
}
static void chart2_ser3_bar3_anim_cb(void * var, int32_t v){
    lv_chart_set_value_by_id(chart2, ser3, 3, v);
}


#endif
