#include "plot.h"

double f(double x) {
    return 0.5 * sin(x) + 0.5 * cos(2 * x) + 0.5 * sin(3 * x) + 0.5 * cos(4 * x);
}

int main() {
    Plot plt;

    plt.graph(f);

    // show the plot
    plt.show();
}