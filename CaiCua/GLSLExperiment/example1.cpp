﻿//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/
#include  "math.h"

// remember to prototype
void generateGeometry( void );
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );


typedef vec4 point4;
typedef vec4 color4;
using namespace std;

// Số các đỉnh của các tam giác
const int NumPoints = 36;

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/


point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program;

void initCube()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
	vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
	vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
	vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
	vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
	vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
	vertices[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colors[6] = color4(1.0, 0.5, 0.0, 1.0); // orange
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
int Index = 0;
void quad(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[d]; Index++;
}
void makeColorCube(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/

{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
void generateGeometry( void )
{
	initCube();
	makeColorCube();
}


void initGPUBuffers( void )
{
	// Tạo một VAO - vertex array object
	GLuint vao;
    glGenVertexArrays( 1, &vao );     
    glBindVertexArray( vao );

    // Tạo và khởi tạo một buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), NULL, GL_STATIC_DRAW );

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors); 
	
}


void shaderSetup( void )
{
	// Nạp các shader và sử dụng chương trình shader
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );   // hàm InitShader khai báo trong Angel.h
    glUseProgram( program );

    // Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
    GLuint loc_vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( loc_vPosition );
    glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

    glClearColor( 1.0, 1.0, 1.0, 1.0 );        /* Thiết lập màu trắng là màu xóa màn hình*/
}

mat4 ctm;

/*void canhcua(GLfloat x, GLfloat y, GLfloat z) {
	mat4 m = Translate(x, y, z) * Scale(0.5, 0.7, 0.05);
	m = ctm * m;
	GLint matrix_loc = glGetUniformLocation(program, "model_view");
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    

}
void khungcua(GLfloat x, GLfloat y,GLfloat z) {
	mat4 m = Translate(x, y, z)*Scale(0.05, 0.8, 0.05);
	m = ctm * m;
	GLint matrix_loc = glGetUniformLocation(program, "model_view");
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
GLfloat  amphax=0,amphay =0, ampha1=0, ampha2 = 0,X=0,Y=0,Z=0;
void canhcua1(GLfloat m, GLfloat n, GLfloat p) {
	ctm = Translate(m, n, p)*RotateX(amphax)*RotateY(amphay);
	khungcua(0,0,0);
	ctm = ctm* RotateY(ampha1);
	canhcua(0.25,0,0);
	//ctm = Translate(m, n, p)* RotateY(ampha1);
}
void canhcua2(GLfloat m, GLfloat n, GLfloat p) {
	ctm = Translate(m, n, p)*RotateY(180)*RotateX(amphax)*RotateY(amphay);
	khungcua(0, 0, 0);
	ctm = ctm * RotateY(ampha1);
	canhcua(0.25, 0, 0);
	//ctm = Translate(m, n, p)* RotateY(ampha1);
}

void doublecua() {
	canhcua1(-0.5, 0, 0);
	canhcua2(0.5, 0, 0);
}*/
void chan(GLfloat a, GLfloat b, GLfloat c) {
	mat4 m = Translate(a, b, c)*Translate(0, 0.12, 0.19)*Scale(0.02, 0.24, 0.02);
	GLint matrix_loc = glGetUniformLocation(program, "model_view");
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, ctm * m);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mat4 m1 = Translate(a, b, c)*Translate(0, 0.12, -0.19)*Scale(0.02, 0.24, 0.02);
	GLint matrix_loc = glGetUniformLocation(program, "model_view");
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, ctm * m1);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mat4 m2 = Translate(a, b, c)*Translate(0, 0.12, 0)*Scale(0.01, 0.02, 0.4);
	GLint matrix_loc = glGetUniformLocation(program, "model_view");
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, ctm * m2);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void display( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//ctm = ctm * Translate(X, Y, Z);

	//doublecua();
	chan(0,0,0);
	

	glutSwapBuffers();	

}


void keyboard( unsigned char key, int x, int y )
{
	// keyboard handler

   switch ( key ) {
/*	case 'k':
		
		if (z2 <= 0.2)
			z2 += 0.02;
		glutPostRedisplay();
		break; 
	case 'K':
		if (z2 >= 0.02)
			z2 -= 0.02;
		glutPostRedisplay();
		break;*/
	/*case 'q':
		if (ampha1 < 90)
			ampha1 += 5;
		glutPostRedisplay();
		break;
	case 'Q':
		if (ampha1 > 0) {
			ampha1 -= 5;
		}
		glutPostRedisplay();
		break;
	
	case 'X':
		if (amphax <90) {
			amphax += 5;
		}
		glutPostRedisplay();
		break;
	case 'x':
		if (amphax > 0)
			amphax -= 5;
		glutPostRedisplay();
		break;
	case 'd':
		X += 5;
		glutPostRedisplay();
		break;*/

	case 033:
			exit(1);
			break;
    }
}


int main( int argc, char **argv )
{
	// main function: program starts here
    glutInit( &argc, argv );                       
    glutInitDisplayMode( GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize( 640, 640 );                 
	glutInitWindowPosition(100,150);               
    glutCreateWindow( "Drawing a cube" );            

   
	glewInit();										

    generateGeometry( );                          
    initGPUBuffers( );							   
    shaderSetup( );                               

    glutDisplayFunc( display );                   
	glutKeyboardFunc(keyboard);

	glutMainLoop();
    return 0;
}