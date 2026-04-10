#ifndef PAKC_ESP_H
#define PAKC_ESP_H

#include <jni.h>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/mman.h>
#include <android/log.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cerrno>
#include <sys/un.h>
#include <cstring>
#include <string>
#include <cmath>

class Color {
public:
	float r;
	float g;
	float b;
	float a;

	Color() {
		this->r = 0;
		this->g = 0;
		this->b = 0;
		this->a = 0;
	}

	Color(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Color(float r, float g, float b) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 255;
	}

	static Color White(int alpha){
		return Color(255, 255, 255, alpha);
	}
	static Color Green(int alpha){
		return Color(0,255,0, alpha);
	}
	static Color Wood(int alpha){
		return Color(40,252,80, alpha);
	}
	static Color Black(int alpha){
		return Color(0,0,0, alpha);
	}
	static Color Red(int alpha)
	{
		return Color(255, 0, 0, alpha);
	}
	static Color Yellow(int alpha)
	{
		return Color(255, 255, 0, alpha);
	}
	static Color Orange(int alpha)
	{
		return Color(255, 165, 0, alpha);
	}
	static Color Blue(int alpha)
	{
		return Color(45, 255, 220, alpha);
	}
};

class Rect {
    public:
    union {
        struct {
            float x;
            float y;
            float w;
            float h;
        };
        float data[4];
    };
    Rect()
            : x(0)
            , y(0)
            , w(0)
            , h(0)
    {
    }
    Rect(float x1, float y1, float w1, float h1)
            : x(x1)
            , y(y1)
            , w(w1)
            , h(h1)
    {
    }
    Rect(const Rect& v);
    ~Rect();
};
Rect::Rect(const Rect& v)
        : x(v.x)
        , y(v.y)
        , w(v.w)
        , h(v.h)
{
}
Rect::~Rect() {}

class ESP
{
  private:
	JNIEnv * _env;
	jobject _cvsView;
	jobject _cvs;


  public:
	  ESP()
	{
		_env = nullptr;
		_cvsView = nullptr;
		_cvs = nullptr;
	}

	ESP(JNIEnv * env, jobject cvsView, jobject cvs)
	{
		this->_env = env;
		this->_cvsView = cvsView;
		this->_cvs = cvs;
	}

	bool isValid() const
	{
		return (_env != nullptr && _cvsView != nullptr && _cvs != nullptr);
	}

	int getWidth() const
	{
		if (isValid())
		{
			jclass canvas = _env->GetObjectClass(_cvs);
			jmethodID width = _env->GetMethodID(canvas, "getWidth", "()I");
			return _env->CallIntMethod(_cvs, width);
		}
		return 0;
	}

	int getHeight() const
	{
		if (isValid())
		{
			jclass canvas = _env->GetObjectClass(_cvs);
			jmethodID width = _env->GetMethodID(canvas, "getHeight", "()I");
			return _env->CallIntMethod(_cvs, width);
		}
		return 0;
	}
	
	void ResetItemCount()
	{
		if (isValid())
		{
			jclass canvasView = _env->GetObjectClass(_cvsView);
			jmethodID drawtext = _env->GetMethodID(canvasView, 
											"ResetItemCount", "()V");
			_env->CallVoidMethod(_cvsView, drawtext);
			
		}
	}

	void DrawLine(Color color, float thickness, Vector2 start, Vector2 end)
	{
		if (isValid())
		{
			jclass canvasView = _env->GetObjectClass(_cvsView);
			jmethodID drawline = _env->GetMethodID(canvasView, "DrawLine",
												   "(Landroid/graphics/Canvas;IIIIFFFFF)V");
			_env->CallVoidMethod(_cvsView, drawline, _cvs, (int)color.a, (int)color.r,
								 (int)color.g, (int)color.b,
								 thickness, start.X, start.Y, end.X, end.Y);
		}
	}
    
	void DrawRect(Color color, float thickness, Vector2 start, Vector2 end)
	{
		if (isValid())
		{
			jclass canvasView = _env->GetObjectClass(_cvsView);
			jmethodID drawline = _env->GetMethodID(canvasView, "DrawRect",
												   "(Landroid/graphics/Canvas;IIIIFFFFF)V");
			_env->CallVoidMethod(_cvsView, drawline, _cvs, (int)color.a, (int)color.r,
								 (int)color.g, (int)color.b,
								 thickness, start.X, start.Y, end.X, end.Y);
		}
	}

	void DrawFilledRect(Color color, Vector2 start, Vector2 end)
	{
		if (isValid())
		{
			jclass canvasView = _env->GetObjectClass(_cvsView);
			jmethodID drawline = _env->GetMethodID(canvasView, "DrawFilledRect",
												   "(Landroid/graphics/Canvas;IIIIFFFF)V");
			_env->CallVoidMethod(_cvsView, drawline, _cvs, (int)color.a, (int)color.r,
								 (int)color.g, (int)color.b, start.X, start.Y, end.X, end.Y);
		}
	}

	void DrawText(Color color, const char *txt, Vector2 pos, float size) {
        if (isValid()) {
			jclass cls = _env->GetObjectClass(_cvsView);
            jmethodID drawtext = _env->GetMethodID(cls, "DrawText",
                                                   "(Landroid/graphics/Canvas;IIIILjava/lang/String;FFF)V");
            jstring s=_env->NewStringUTF(txt);
            _env->CallVoidMethod(_cvsView, drawtext, _cvs, (int) color.a, (int) color.r,
                                 (int) color.g, (int) color.b,
                                 s, pos.X, pos.Y, size);
            _env->DeleteLocalRef(s);
        }
    }
	
	void DrawFilledCircle(Color color, Vector2 pos, float radius)
	{
		if (isValid())
		{
			jclass canvasView = _env->GetObjectClass(_cvsView);
			jmethodID drawfilledcircle = _env->GetMethodID(canvasView, "DrawFilledCircle",
														   "(Landroid/graphics/Canvas;IIIIFFF)V");
			_env->CallVoidMethod(_cvsView, drawfilledcircle, _cvs, (int)color.a, (int)color.r,
								 (int)color.g, (int)color.b, pos.X, pos.Y, radius);
		}
	}

	void DrawCircle(Color color, Vector2 pos, float radius, float thickness)
	{
		if (isValid())
		{
			jclass canvasView = _env->GetObjectClass(_cvsView);
			jmethodID drawcircle = _env->GetMethodID(canvasView, "DrawCircle",
													 "(Landroid/graphics/Canvas;IIIIFFFF)V");
			_env->CallVoidMethod(_cvsView, drawcircle, _cvs, (int)color.a, (int)color.r,
								 (int)color.g, (int)color.b, pos.X, pos.Y, radius, thickness);
		}
	}
	
	void DrawOTH(Vector2 start, int num) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawline = _env->GetMethodID(canvasView, "DrawOTH",
                                                   "(Landroid/graphics/Canvas;IFF)V");
            _env->CallVoidMethod(_cvsView, drawline, _cvs, num, start.X, start.Y);
        }
    }
};


#endif // PAKC_ESP_H
