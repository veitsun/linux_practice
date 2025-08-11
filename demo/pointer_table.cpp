/**
 * @file pointer_table.cpp
 * @author your name (you@domain.com)
 * @brief 通过函数指针表实现 C 编程的多态性
 * @version 0.1
 * @date 2025-08-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <cmath>
#include <cstdio>
#include <cstdlib>

// 行为接口，所有形状都会实现的“方法”
struct shape_vtable {
	double (*area)(void *self);
	double (*perimeter)(void *self);
	void (*destroy)(void *self);
};

// 基类对象，带一个指向 vtable 的指针 + 公共字段
// 也就是有很多的不同子类，都是基于这个基类对象扩展的
struct shape {
	const shape_vtable *vtable; // 指向虚函数表的指针
};


// 一个矩形的“子类”，继承 base 基类
struct rect {
    struct shape base;  // 必须放在开头，保证布局兼容
    double w, h;
};

// 一个圆的“子类”，继承 base 基类
// 注意这里的布局跟 rect 不一样，rect 是两个 double 字段，circle 是一个 double 字段
// 但是都继承自 shape 基类
struct circle {
    struct shape base;
    double r;
};



// 具体的实现方法，这里跟接口表 vtable 里的 area 函数指针没有任何关系 （vtable -> area）
// 这是一个矩形面积的通用函数，怎么跟接口表里的 area 函数指针扯上关系呢？？
static double rect_area(void *self) {
    struct rect *r = (struct rect *)self;
    return r->w * r->h;
}

static double circle_area(void *self) { return M_PI * ((struct circle*)self)->r * ((struct circle*)self)->r; }


// 这是一个矩形周长的通用计算函数，这里怎么跟接口表里的 area 函数指针扯上关系呢？？
static double rect_perimeter(void *self) {
    struct rect *r = (struct rect *)self;
    return 2 * (r->w + r->h);
}

static double circle_perimeter(void *self) { return 2 * M_PI * ((struct circle*)self)->r; }


// 这是一个矩形销毁的通用函数，这里怎么跟接口表里的 destroy 函数指针扯上关系呢？？
static void rect_destroy(void *self) {
    free(self);
}

static void   circle_destroy(void *self) { free(self); }


// 矩形的虚函数表，该实现的 vtable  （通常声明成 static_const ）
static const shape_vtable RECT_VT = {
    .area = rect_area,
    .perimeter = rect_perimeter,
    .destroy = rect_destroy,
};

static const struct shape_vtable CIRCLE_VT = {
    .area = circle_area,
    .perimeter = circle_perimeter,
    .destroy = circle_destroy,
};

// 创建一个矩形
struct rect *rect_create(double w, double h) {
    struct rect *r = (struct rect *)malloc(sizeof(struct rect));
    r->base.vtable = &RECT_VT;
    r->w = w;
    r->h = h;
    return r;
}

struct circle *circle_new(double r) {
    struct circle *c = (struct circle *) malloc(sizeof *c);
    c->base.vtable = &CIRCLE_VT;
    c->r = r;
    return c;
}

double shape_area   (struct shape *s) { return s->vtable->area((void*)s); }
double shape_perim  (struct shape *s) { return s->vtable->perimeter((void*)s); }
void   shape_destroy(struct shape *s) {        s->vtable->destroy((void*)s); }

int main()
{
	struct shape *a = (struct shape *) rect_create(3, 4);
	struct shape *b = (struct shape *) circle_new(5);
	printf("Area of rectangle: %f\n", shape_area(a));
	printf("Area of circle: %f\n", shape_area(b));
	shape_destroy(a);
	shape_destroy(b);
	return 0;
}