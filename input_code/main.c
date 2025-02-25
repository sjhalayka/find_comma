

float* i;


int func2(void)
{
    char* p;

    p = 0;
    p = 1;
    p = 2;
    p = 3;
    p = 4;
}


int func(void)
{
    i = malloc(sizeof(float) * 10);
    free(i);

    i += 10;
    i--;
}



// double** d;

int main(void) 
{
    
        double** p;
        p = 10;
    
    for (int f = 0; f < 10; f++)
    {
    }

    {
        double* p;
        p = 20;
        p++;
        p--;
    }

    p = 5;
    p--;
    p++;
}
