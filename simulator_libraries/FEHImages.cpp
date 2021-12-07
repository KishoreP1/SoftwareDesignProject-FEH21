#include<FEHLCD.h>
#include<FEHImages.h>
#include<fstream>

FEHIMAGE::FEHIMAGE()
{
	for(int i=0; i<240; i++)
	{
		for(int j=0; j<320; j++)
			saved_image[i][j]=0;
	}
}

void FEHIMAGE::Open(const char * filename)
{

	std::ifstream pic;
	pic.open(filename);
	pic>>rows>>cols;
	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
			pic>>saved_image[i][j];	
	}
	pic.close();
}

//filename is file output by MATLAB to draw. Should end in *FEH.pic
//x,y are top left location of where to draw picture
void FEHIMAGE::Draw(int x, int y)
{
	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
			if(saved_image[i][j]!=-1)
			{
				LCD.SetFontColor(saved_image[i][j]);
				LCD.DrawPixel(j+y,i+x);
			}
		}
	}

}
