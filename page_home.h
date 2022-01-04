
//using page_home_data_t = struct page_home_data_t
//{
//    char         title[18] = {"HOME\nPAGE"};
//    bool         latch = 1;
//    uint32_t     lss = 0xeeeeeeee;
//    const char*  lssFormat = "%4d";
//    uint32_t     slew = 2;
//    const char*  slewFormat = "%3.1f";
//};


//.render = [](terminal_t<T>& terminal, void* pdata)
//{
//    auto& data = *(page_home_data_t*)pdata;
//
//    rect_t rc;
//    terminal.getClientRect(rc);
//
//    // Title
//    if (data.title)
//    {
//        rect_t rcText;
//        terminal.context.calcText(rcText, data.title,
//            horizontal_aligment_t::Center, vertical_aligment_t::Top);
//
//        auto dx = rc.left + (rc.width() - rcText.width()) / 2;
//        auto dy = rc.top + (coord_t)(rc.height() * .25);
//        rcText.offset(dx, dy);
//        terminal.context.text(rcText, data.title, TEXT_COLOR, TEXT_BGCOLOR,
//            horizontal_aligment_t::Center, vertical_aligment_t::Top);
//    }
//
//    // Buttons
//
//    terminal.drawButtons(g_page_home_buttons);
//},


//.input = [](terminal_t<T>& terminal, uint8_t index)
//{
//    switch (index)
//    {
//    case 1:  // A-G
//        break;
//    case 2:  // STBY
//        break;
//    case 3:  // A-A
//        break;
//    case 5:  // SLEW
//    {
//        g_page_input.szTitle = "INPUT\nSLEW";
//        g_page_input.min = 0;
//        g_page_input.max = 99; // 9.9
//        g_page_input.pnValue = &g_page_home.slew;
//        g_page_input.szFormat = "%2.1f";
//        snprintf(g_page_input.szValue, 64, g_page_input.szFormat, (float)g_page_home.slew / 10);
//
//        terminal.navigator.push(1);
//    }
//        break;
//    case 8:  // LATCH
//        break;
//    case 17: // LSS
//        g_page_input.szTitle = "INPUT\nLSS";
//        g_page_input.min = 1000;
//        g_page_input.max = 9999; // 9999
//        g_page_input.pnValue = &g_page_home.lss;
//        g_page_input.szFormat = "%04d";
//        snprintf(g_page_input.szValue, 64, g_page_input.szFormat, g_page_home.lss);
//
//        terminal.navigator.push(1);
//        break;
//    }
//},
