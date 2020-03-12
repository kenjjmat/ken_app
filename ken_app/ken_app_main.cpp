#include "ken_app_main.h"

void ken_app_main::on_caption()
{
}

void ken_app_main::on_stop(){
	stop();
}

void ken_app_main::on_shutdown()
{
}

ken_app_main::ken_app_main(const std::string& guid, state& app_state) :
	home_page_name("Main_form"),
	app_state_(app_state),
	gui::gui(guid){
}

ken_app_main::~ken_app_main()
{
}

bool ken_app_main::layout(gui::page& persistent_page,
	gui::page& home_page, 
	std::string& error) {

	// set parameters
	home_page.set_name(home_page_name);

	// setting the parameters for the window
	set_width(800);
	set_min_width(800);
	set_height(500);
	set_min_height(500);

	// add image for stock
	widgets::image image_stock;
	image_stock.rect.left =50;
	image_stock.rect.top = 20;
	image_stock.rect.set_width(200);
	image_stock.rect.set_height(100);
	image_stock.text = "stock";
	image_stock.text_position = widgets::image_text_placement::right_top;
	image_stock.filename = "stock.png";
	image_stock.alias = "image_stock";
	image_stock.font_size = 10;

	home_page.add_image(image_stock);

	return true;
}
