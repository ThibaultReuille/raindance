#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Debug.hh>

namespace GLUT
{
	class IContext
	{
	public:
	    virtual ~IContext() {}

		virtual void draw() = 0;

	    virtual void reshape(int width, int height)
	    {
	        (void) width;
	        (void) height;
	    }

	    virtual void idle()
	    {
	        glutPostRedisplay();
	    }

	    virtual void finish()
        {
	        checkGLErrors();
            glutSwapBuffers();
        }

	    virtual void keyboard(unsigned char key, int x, int y)
	    {
	        (void) key;
	        (void) x;
	        (void) y;
	    }

	    virtual void keyboardUp(unsigned char key, int x, int y)
	    {
	        (void) key;
	        (void) x;
	        (void) y;
	    }

	    virtual void special(int key, int x, int y)
	    {
	        (void) key;
	        (void) x;
	        (void) y;
	    }

	    virtual void specialUp(int key, int x, int y)
	    {
	        (void) key;
	        (void) x;
	        (void) y;
	    }

	    virtual void mouse(int button, int state, int x, int y)
	    {
	        (void) button;
	        (void) state;
	        (void) x;
	        (void) y;
	    }

	    virtual void motion(int x, int y)
	    {
	        (void) x;
	        (void) y;
	    }
	};

	IContext* g_Context = NULL;

	static void reshape(int width, int height) { g_Context->reshape(width, height); }
	static void draw() { g_Context->draw(); }
	static void idle() { g_Context->idle(); }
	static void keyboard(unsigned char key, int x, int y) { g_Context->keyboard(key, x, y); }
	static void keyboardUp(unsigned char key, int x, int y) { g_Context->keyboardUp(key, x, y); }
	static void special(int key, int x, int y) { g_Context->special(key, x, y); }
	static void specialUp(int key, int x, int y) { g_Context->specialUp(key, x, y); }
	static void mouse(int button, int state, int x, int y) { g_Context->mouse(button, state, x, y); }
	static void motion(int x, int y) { g_Context->motion(x, y); }

	static void setContext(IContext* context)
	{
		g_Context = context;
	}

	static void setCallbacks()
	{
		glutReshapeFunc(reshape);
		glutDisplayFunc(draw);
		glutIdleFunc(idle);

		glutKeyboardFunc(keyboard);
		glutKeyboardUpFunc(keyboardUp);
		glutSpecialFunc(special);
		glutSpecialUpFunc(specialUp);
		glutMouseFunc(mouse);
		glutMotionFunc(motion);
	}
}

