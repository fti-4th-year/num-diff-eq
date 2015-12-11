#include <cstdio>
#include <cmath>
#include <functional>

class Solver {
public:
	virtual double get_dy(std::function<double(double,double)> deriv, double x, double y, double dx) const = 0;
};

class EulerSolver : public Solver {
public:
	virtual double get_dy(std::function<double(double,double)> deriv, double x, double y, double dx) const override {
		return deriv(x, y)*dx;
	}
};

class EulerModifiedSolver : public Solver {
public:
	virtual double get_dy(std::function<double(double,double)> deriv, double x, double y, double dx) const override {
		return 0.5*dx*(deriv(x, y) + deriv(x + dx, y + dx*deriv(x, y)));
	}
};

class RungeKuttaSolver : public Solver {
public:
	virtual double get_dy(std::function<double(double,double)> deriv, double x, double y, double dx) const override {
		double k[4];
		k[0] = deriv(x, y);
		k[1] = deriv(x + 0.5*dx, y + 0.5*dx*k[0]);
		k[2] = deriv(x + 0.5*dx, y + 0.5*dx*k[1]);
		k[3] = deriv(x + dx, y + dx*k[2]);
		return dx/6.0*(k[0] + 2.0*k[1] + 2.0*k[2] + k[3]);
	}
};

class Sampler {
public:
	double x0, x1;
	double y0;
	int mpow;
	std::function<double(double)> func;
	std::function<double(double, double)> deriv;
	
	void sample(const Solver &solver, const std::string &filename) {
		FILE *file;
		file = fopen(filename.c_str(), "w");
		
		// Solution
		{
			double dx = 1.1;
			double x = x0, y = y0;
			for(; x <= x1;) {
				y += solver.get_dy(deriv, x, y, dx);
				x += dx;
				fprintf(file, "%.16le %.16le\n", x, y);
			}
		}
		
		/*
		//Error Regression
		for(int i = 1; i < mpow; ++i) {
			double dx = 1.0/(1 << i);
			double x = x0, y = y0;
			for(; x <= x1;) {
				y += solver.get_dy(deriv, x, y, dx);
				x += dx;
			}
			fprintf(file, "%.16le\n", fabs(func(x) - y));
		}
		*/
		
		fclose(file);
	}
};

void print(FILE *file, double x, double y, std::function<double(double)> func) {
	fprintf(file, "%.16le %.16le %.16le\n", x, y, fabs((func(x) - y)));
}

int main(int argc, char *argv[]) {
	Sampler sampler;
	sampler.x0 = 0.0;
	sampler.x1 = 100.0; //3.0;
	sampler.y0 = 1.0 + 1e-8;
	sampler.mpow = 0x14;
	sampler.func = [](double x) {return 1.0 + x;}; //{return exp(-x);};
	sampler.deriv = [](double x, double y) {return y - x;}; //{return -y;};
	
	sampler.sample(EulerSolver(), "euler.txt");
	sampler.sample(EulerModifiedSolver(), "euler-mod.txt");
	sampler.sample(RungeKuttaSolver(),    "runge-kutta.txt");
	
	return 0;
}
