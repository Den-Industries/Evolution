#pragma once
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

// ============================================================
//  »нстансный рендер кругов с заливкой и обводкой
// ============================================================
class CircleRenderer
{
public:
	struct Inst {
		float x, y;            // центр
		float radius;          // радиус (внешний, без обводки)
		float outline;         // толщина обводки (в мировых ед.)
		float fr, fg, fb, fa;  // fill rgba 0..1
		float or_, og, ob, oa; // outline rgba 0..1
	};

	void init()
	{
		// геометри€: quad [-1..1] (covers круг + обводку через uv)
		float quad[] = {
			-1.f,-1.f,  1.f,-1.f,  1.f, 1.f,
			-1.f,-1.f,  1.f, 1.f, -1.f, 1.f
		};

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vboQuad);
		glBindBuffer(GL_ARRAY_BUFFER, vboQuad);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

		glGenBuffers(1, &vboInst);
		glBindBuffer(GL_ARRAY_BUFFER, vboInst);

		const GLsizei stride = sizeof(Inst);
		// loc1: pos+radius+outline (vec4)
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Inst, x));
		glVertexAttribDivisor(1, 1);
		// loc2: fill rgba
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Inst, fr));
		glVertexAttribDivisor(2, 1);
		// loc3: outline rgba
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Inst, or_));
		glVertexAttribDivisor(3, 1);

		glBindVertexArray(0);

		prog = buildProgram(
			// VERTEX
			"#version 330 core\n"
			"layout(location=0) in vec2 aQuad;\n"
			"layout(location=1) in vec4 aPosRad;\n"   // xy=pos, z=radius, w=outline
			"layout(location=2) in vec4 aFill;\n"
			"layout(location=3) in vec4 aOutline;\n"
			"uniform mat4 uProj;\n"
			"out vec2 vUv;\n"
			"out float vRadius;\n"
			"out float vOutline;\n"
			"out vec4 vFill;\n"
			"out vec4 vOutlineCol;\n"
			"void main(){\n"
			"  float R = aPosRad.z + max(aPosRad.w,0.0);\n" // внешний размер с обводкой
			"  vec2 world = aPosRad.xy + aQuad * R;\n"
			"  vUv = aQuad * R;\n"               // координата от центра в мир.ед.
			"  vRadius = aPosRad.z;\n"
			"  vOutline = aPosRad.w;\n"
			"  vFill = aFill;\n"
			"  vOutlineCol = aOutline;\n"
			"  gl_Position = uProj * vec4(world,0.0,1.0);\n"
			"}\n",
			// FRAGMENT
			"#version 330 core\n"
			"in vec2 vUv;\n"
			"in float vRadius;\n"
			"in float vOutline;\n"
			"in vec4 vFill;\n"
			"in vec4 vOutlineCol;\n"
			"out vec4 frag;\n"
			"void main(){\n"
			"  float d = length(vUv);\n"
			"  float outer = vRadius + max(vOutline,0.0);\n"
			"  if (d > outer) discard;\n"
			"  float aa = fwidth(d);\n"
			"  if (vOutline > 0.0001 && d > vRadius){\n"
			"     float a = vOutlineCol.a * (1.0 - smoothstep(outer-aa, outer, d));\n"
			"     frag = vec4(vOutlineCol.rgb, a);\n"
			"  } else {\n"
			"     float edge = (vOutline>0.0001)? vRadius : outer;\n"
			"     float a = vFill.a * (1.0 - smoothstep(edge-aa, edge, d));\n"
			"     frag = vec4(vFill.rgb, a);\n"
			"  }\n"
			"}\n"
		);
		uProj = glGetUniformLocation(prog, "uProj");
	}

	void add(float x, float y, float radius,
		const sf::Color& fill,
		float outline = 0.f,
		const sf::Color& outlineCol = sf::Color::Transparent)
	{
		Inst in;
		in.x = x; in.y = y; in.radius = radius; in.outline = outline;
		in.fr = fill.r / 255.f; in.fg = fill.g / 255.f; in.fb = fill.b / 255.f; in.fa = fill.a / 255.f;
		in.or_ = outlineCol.r / 255.f; in.og = outlineCol.g / 255.f; in.ob = outlineCol.b / 255.f; in.oa = outlineCol.a / 255.f;
		data.push_back(in);
	}

	// proj Ч матрица 4x4 (column-major)
	void flush(const float* proj)
	{
		if (data.empty()) return;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(prog);
		glUniformMatrix4fv(uProj, 1, GL_FALSE, proj);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vboInst);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Inst), data.data(), GL_DYNAMIC_DRAW);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (GLsizei)data.size());

		glBindVertexArray(0);
		glUseProgram(0);
		data.clear();
	}

	void clear() { data.clear(); }
	size_t count() const { return data.size(); }

private:
	GLuint vao = 0, vboQuad = 0, vboInst = 0, prog = 0;
	GLint uProj = -1;
	std::vector<Inst> data;

	static GLuint compile(GLenum type, const char* src)
	{
		GLuint s = glCreateShader(type);
		glShaderSource(s, 1, &src, nullptr);
		glCompileShader(s);
		GLint ok; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
		if (!ok) { char log[1024]; glGetShaderInfoLog(s, 1024, nullptr, log); std::cerr << "Shader err: " << log << "\n"; }
		return s;
	}
	static GLuint buildProgram(const char* vs, const char* fs)
	{
		GLuint v = compile(GL_VERTEX_SHADER, vs);
		GLuint f = compile(GL_FRAGMENT_SHADER, fs);
		GLuint p = glCreateProgram();
		glAttachShader(p, v); glAttachShader(p, f);
		glLinkProgram(p);
		GLint ok; glGetProgramiv(p, GL_LINK_STATUS, &ok);
		if (!ok) { char log[1024]; glGetProgramInfoLog(p, 1024, nullptr, log); std::cerr << "Link err: " << log << "\n"; }
		glDeleteShader(v); glDeleteShader(f);
		return p;
	}
};

// ============================================================
//  »нстансный рендер повЄрнутых пр€моугольников (жгутики)
// ============================================================
class RectRenderer
{
public:
	struct Inst {
		float cx, cy;          // центр в мире
		float hw, hh;          // полу-ширина, полу-высота
		float cosr, sinr;      // поворот
		float fr, fg, fb, fa;  // fill
	};

	void init()
	{
		float quad[] = {
			-1.f,-1.f,  1.f,-1.f,  1.f, 1.f,
			-1.f,-1.f,  1.f, 1.f, -1.f, 1.f
		};
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vboQuad);
		glBindBuffer(GL_ARRAY_BUFFER, vboQuad);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

		glGenBuffers(1, &vboInst);
		glBindBuffer(GL_ARRAY_BUFFER, vboInst);
		const GLsizei stride = sizeof(Inst);
		glEnableVertexAttribArray(1); // cx,cy,hw,hh
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Inst, cx));
		glVertexAttribDivisor(1, 1);
		glEnableVertexAttribArray(2); // cos,sin + pad
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Inst, cosr));
		glVertexAttribDivisor(2, 1);
		glEnableVertexAttribArray(3); // fill
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Inst, fr));
		glVertexAttribDivisor(3, 1);
		glBindVertexArray(0);

		prog = CircleRenderer_buildProgram(
			"#version 330 core\n"
			"layout(location=0) in vec2 aQuad;\n"
			"layout(location=1) in vec4 aRect;\n"   // cx cy hw hh
			"layout(location=2) in vec2 aRot;\n"    // cos sin
			"layout(location=3) in vec4 aFill;\n"
			"uniform mat4 uProj;\n"
			"out vec4 vFill;\n"
			"void main(){\n"
			"  vec2 local = aQuad * aRect.zw;\n"
			"  vec2 r = vec2(local.x*aRot.x - local.y*aRot.y, local.x*aRot.y + local.y*aRot.x);\n"
			"  vec2 world = aRect.xy + r;\n"
			"  vFill = aFill;\n"
			"  gl_Position = uProj * vec4(world,0.0,1.0);\n"
			"}\n",
			"#version 330 core\n"
			"in vec4 vFill;\n"
			"out vec4 frag;\n"
			"void main(){ frag = vFill; }\n"
		);
		uProj = glGetUniformLocation(prog, "uProj");
	}

	// pos = центр стороны креплени€, length вдоль угла degAngle, ширина height
	// ƒл€ жгутиков: origin у теб€ был у кра€, поэтому делаем центр со сдвигом.
	void add(float cx, float cy, float halfW, float halfH, float degAngle, const sf::Color& c)
	{
		Inst in;
		float rad = degAngle * 0.01745329f;
		in.cosr = cosf(rad); in.sinr = sinf(rad);
		in.cx = cx; in.cy = cy; in.hw = halfW; in.hh = halfH;
		in.fr = c.r / 255.f; in.fg = c.g / 255.f; in.fb = c.b / 255.f; in.fa = c.a / 255.f;
		data.push_back(in);
	}

	void flush(const float* proj)
	{
		if (data.empty()) return;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUseProgram(prog);
		glUniformMatrix4fv(uProj, 1, GL_FALSE, proj);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vboInst);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Inst), data.data(), GL_DYNAMIC_DRAW);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (GLsizei)data.size());
		glBindVertexArray(0);
		glUseProgram(0);
		data.clear();
	}

private:
	GLuint vao = 0, vboQuad = 0, vboInst = 0, prog = 0;
	GLint uProj = -1;
	std::vector<Inst> data;

	static GLuint CircleRenderer_buildProgram(const char* vs, const char* fs)
	{
		auto compile = [](GLenum t, const char* s) {
			GLuint sh = glCreateShader(t);
			glShaderSource(sh, 1, &s, nullptr); glCompileShader(sh);
			GLint ok; glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
			if (!ok) { char log[1024]; glGetShaderInfoLog(sh, 1024, nullptr, log); std::cerr << "Shader err: " << log << "\n"; }
			return sh;
			};
		GLuint v = compile(GL_VERTEX_SHADER, vs), f = compile(GL_FRAGMENT_SHADER, fs);
		GLuint p = glCreateProgram(); glAttachShader(p, v); glAttachShader(p, f); glLinkProgram(p);
		GLint ok; glGetProgramiv(p, GL_LINK_STATUS, &ok);
		if (!ok) { char log[1024]; glGetProgramInfoLog(p, 1024, nullptr, log); std::cerr << "Link err: " << log << "\n"; }
		glDeleteShader(v); glDeleteShader(f);
		return p;
	}
};

// ===== глобальные рендеры =====
CircleRenderer gCircles;
RectRenderer   gRects;

// ѕостроить ортопроекцию из sf::View (центр+размер), Y вниз Ч как в SFML
inline void BuildProjFromView(const sf::View& v, float* m)
{
	sf::Vector2f c = v.getCenter();
	sf::Vector2f s = v.getSize();
	float l = c.x - s.x / 2.f, r = c.x + s.x / 2.f;
	float t = c.y - s.y / 2.f, b = c.y + s.y / 2.f; // top<bottom, Y вниз
	// ortho(l,r,b,t): но чтобы Y шЄл вниз как в SFML, делаем top->-1? нет:
	// хотим: x=l -> -1, x=r -> +1 ; y=t -> +1, y=b -> -1  (экран: верх = +1)
	for (int i = 0; i < 16; i++) m[i] = 0;
	m[0] = 2.f / (r - l);
	m[5] = -2.f / (b - t);   // минус: Y вниз
	m[10] = -1.f;
	m[12] = -(r + l) / (r - l);
	m[13] = (b + t) / (b - t);
	m[15] = 1.f;
}