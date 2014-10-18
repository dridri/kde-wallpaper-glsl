#include <libge/libge.h>

static ge_Shader* m_shader = 0;
static int loc_resolution = -1;

void loadShader()
{
	if(m_shader){
		geFreeShader(m_shader);
	}
	m_shader = geCreateShader();
	geShaderLoadVertexSource(m_shader, "drich-wallpaper.vert");
	geShaderLoadFragmentSource(m_shader, "drich-wallpaper.frag");
	m_shader->loc_time = geShaderUniformID(m_shader, "time");
	loc_resolution = geShaderUniformID(m_shader, "resolution");
}

int main()
{
	geDebugMode(GE_DEBUG_ALL);
	geInit();
	geCreateMainWindow("Wallpaper", 600, 480, GE_WINDOW_RESIZABLE);

	loadShader();

	ge_Keys* keys = geCreateKeys();

	while(1)
	{
		geReadKeys(keys);
		if(keys->pressed['R'])
		{
			loadShader();
		}

		geClearScreen();
		geShaderUse(m_shader);
		geShaderUniform1f(m_shader->loc_time, ((float)geGetTick()) / 1000.0f);
		geShaderUniform2f(loc_resolution, geGetContext()->width, geGetContext()->height);
		geUpdateMatrix();
		geFillRectScreen(0, 0, geGetContext()->width, geGetContext()->height, 0xFFFFFFFF);
		geSwapBuffers();
	}
}