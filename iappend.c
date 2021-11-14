char *iappendd(char *buf,unsigned long long int a,unsigned int nmin,unsigned int nmax)
{
	unsigned long long int a1;
	int x;
	if(nmax<20)
	{
		a1=1;
		x=nmax;
		while(x>0)
		{
			a1*=10;
			x--;
		}
		a%=a1;
		a1/=10;
	}
	else
	{
		a1=10000000000000000000UL;
		nmax=20;
	}
	x=nmax;
	while(a1>a&&x>nmin)
	{
		a1/=10;
		x--;
	}
	x=strlen(buf);
	while(a1)
	{
		buf[x]=a/a1+'0';
		a%=a1;
		a1/=10;
		x++;
	}
	buf[x]=0;
	return buf;
}
