
class FEHIMAGE
{
	public:
		FEHIMAGE();
		void Open(const char *);
		void Draw(int, int);
	private:
		int saved_image[240][320];
		int rows,cols;
};
