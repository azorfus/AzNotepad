#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

static Fl_Double_Window* window = new Fl_Double_Window(500, 400, "AzNotepad");
static std::string file_name;
static Fl_Text_Editor* edit = new Fl_Text_Editor(10, 10, 480, 360);
static Fl_Text_Buffer* buffer = new Fl_Text_Buffer();

void load_file(const char* filename)
{
	file_name = filename;
	std::ifstream file;
	std::stringstream buffer_c;
	buffer->remove(0, buffer->length());
	file.open(filename);
	if(!file)
	{
		fl_message("Error opening file");
	}
	else
	{
		buffer_c << file.rdbuf();
		file.close();
		buffer->text(buffer_c.str().c_str());
	}
}

void save_file(const char* filename)
{
	std::ofstream file;
	file.open(filename, std::ios::trunc | std::ios::in);
	if(!file)
	{
		fl_message("Error save file");
	}
	else
	{
		file << buffer->text();
		file.close();
	}
}

void load_cb(Fl_Widget*)
{
	char buff[100];
	if(buffer->text() != 0)
	{	
		Fl_Native_File_Chooser fl_chooser;
		int r = fl_choice("The current file has not been saved.\n"
						  "Would you like to save it now?",
        	              "Cancel", "Save and Load", "Don't Save");
		if(r==1)
		{
			if(file_name=="")
			{
				fl_chooser.title("Save file");
				fl_chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
				if(fl_chooser.show()) return;
				save_file(fl_chooser.filename());
				fl_chooser.title("Load file");
				fl_chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
				if(fl_chooser.show()) return;
				load_file(fl_chooser.filename());
			}
			else
			{
				save_file(file_name.c_str());
				fl_chooser.title("Load file");
				fl_chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
				if(fl_chooser.show()) return;
				load_file(fl_chooser.filename());
			}
		}
		else if(r==2)
		{
			fl_chooser.title("Load file");
			fl_chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
			if(fl_chooser.show()) return;
			load_file(fl_chooser.filename());
			snprintf(buff, sizeof(buff), "AzNotes - %s", fl_chooser.filename());
			window->label(buff);
		}
	}
	else
	{
		Fl_Native_File_Chooser fl_chooser;
		fl_chooser.title("Load file");
		fl_chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
		if(fl_chooser.show()) return;
		load_file(fl_chooser.filename());
		snprintf(buff, sizeof(buff), "AzNotes - %s", fl_chooser.filename());
		window->label(buff);
	}
}

void save_cb(Fl_Widget*)
{
	if(file_name!="")
	{
		save_file(file_name.c_str());
	}
	else
	{
		Fl_Native_File_Chooser fl_chooser;
		fl_chooser.title("Save file");
		fl_chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
		if(fl_chooser.show()) return;
		save_file(fl_chooser.filename());
	}
}

void saveas_cb(Fl_Widget*)
{
	Fl_Native_File_Chooser fl_chooser;
	fl_chooser.title("Save file");
	fl_chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	if(fl_chooser.show()) return;
	save_file(fl_chooser.filename());
}

void quit_cb(Fl_Widget*)
{
	int r = fl_choice("The current file has not been saved.\n"
                      "Would you like to save it now?",
                      "Cancel", "Save and Quit", "Don't Save");
	if(r==1)
	{
		if(file_name!="")
		{
			save_file(file_name.c_str());
		}
		else
		{
			Fl_Native_File_Chooser fl_chooser;
			fl_chooser.title("Save file");
			fl_chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
			if(fl_chooser.show()) return;
			save_file(fl_chooser.filename());
			exit(0);
		}
	}
	else if(r==2)
	{
		exit(0);
	}
}

int main()
{
	window->size_range(400, 300);
	edit->buffer(buffer);
	edit->textsize(12);
	buffer->tab_distance(4);

	Fl_Button* load = new Fl_Button(10, 375, 60, 20, "Load");
	load->labelsize(12);
	Fl_Button* save = new Fl_Button(70, 375, 60, 20, "Save");
	save->labelsize(12);
	Fl_Button* saveas = new Fl_Button(130, 375, 60, 20, "Save As");
	saveas->labelsize(12);
	Fl_Button* quit = new Fl_Button(190, 375, 60, 20, "Quit");
	quit->labelsize(12);

	load->callback(load_cb);
	save->callback(save_cb);
	saveas->callback(saveas_cb);
	quit->callback(quit_cb);

	window->resizable(edit);
	window->show();
	return Fl::run();
}
