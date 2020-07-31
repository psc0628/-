#include<bits/stdc++.h>
typedef long long LL;
using namespace std;

LL n,c;
LL a[1010];
LL d[1010];
LL y[1010];
LL P[1010][1010];
//[D0,P]
//真实的P从第二列开始，第一列存放D0

void debug()
{
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<=n;j++)
            cout<<P[i][j]<<' ';
        cout<<endl;
    }
    cout<<endl;
}

LL gcd(LL a, LL b)
{
    return b? gcd(b, a % b) : a;
}

//用于求解ax+by=gcd(a,b)
void extend_Euclid(LL a, LL b, LL &x, LL &y)
{
    if(b == 0)
    {
        x = 1;
        y = 0;
        return;
    }
    extend_Euclid(b, a % b, x, y);
    LL tmp = x;
    x = y;
    y = tmp - (a / b) * y;
}

int main()
{
    cout<<"请输入项数n,ai,c(a1x1+a2x2+...+anxn=c):"<<endl;
	while(cin>>n)
	{
	    if(n<2 || n>1000)
        {
            cout<<"what do you want to do?"<<endl;
            continue;
	    }
	    //a1x1+a2x2+...+anxn=c
        for(int i=0;i<n;i++)
            cin>>a[i];
        cin>>c;
        //求gcd(a1,a2,...,an)
        //顺便求得d[i]=gcd(d[i-1],ai)
        d[2]=gcd(a[0],a[1]);
        if(d[2]<0) d[2]=-d[2];
        for(int i=2;i<n;i++)
        {
            d[i+1]=gcd(d[i],a[i]);
            if(d[i+1]<0) d[i+1]=-d[i+1];
        }
        LL d0=d[n];
        //有解条件是d0|c
        if(c%d0!=0)
        {
            cout<<"此不定方程无解"<<endl;
            continue;
        }
        //令a1=k1d0,a2=k2d0,...,an=knd0
        //为使得k1d0位置变为d0，即求不定方程
        //k1y1+k2y2+...+knyn=1,其中gcd(k1,k2,...,kn)=1
        //令k1y1+k2y2=d2t2,t2+k3y3=d3t3,...,dn-1tn-1+knyn=1or-1
        //此处di指ki间,故可知为d[i]/d0;由k1!=0可知ti!=0
        LL t,temp=1;
        for(int i=n-1;i>=1;i--)
        {
            if(i==1) extend_Euclid(a[0]/d0,a[1]/d0,y[0],y[1]);
            else extend_Euclid(d[i]/d0,a[i]/d0,t,y[i]);
            //防止k1为0，ax+by=gcd(a,b),x=x0+b/g,y=y0-a/g
            if(i==1 && y[0]==0) y[0]+=a[1]/d[2],y[1]-=a[0]/d[2];
            if(i!=1 && t==0)  t+=a[i]/d[i+1],y[i]-=d[i]/d[i+1];
            //本次解*=上次ti
            y[i]*=temp;
            if(i==1) y[0]*=temp;
            temp=t;
            //cout<<t<<' '<<y[i]<<endl;
        }
        //cout<<y[0]<<endl;
        //求得的y[i]即是一组特解
        cout<<"一组特解为 O（nlogn）："<<endl;
        LL tmp=0;
        for(int i=0;i<n;i++)
            tmp+=y[i]*a[i];
        //和为-1同乘-1即可
        for(int i=0;i<n;i++)
            cout<<"x"<<i+1<<"= "<<tmp*c/d0*y[i]<<endl;
        cout<<endl;

        //构造[D0,P]
        for(int i=0;i<n;i++)
        {
            P[i][0]=a[i];
            for(int j=1;j<=n;j++)
                if(j==i+1) P[i][j]=1;
                else P[i][j]=0;
        }
        //debug();
        //利用已求得的yi进行初等行变换
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<=n;j++)
            {
                if(i==0) P[i][j]=P[i][j]*y[i];
                else P[0][j]+=P[i][j]*y[i];
            }
        }
        //若求得的为-d0，则第一行*-1
        if(P[0][0]==-d0)
            for(int j=0;j<=n;j++)
                P[0][j]*=-1;
        //debug();
        //利用第一行去消下面的行
        for(int i=1;i<n;i++)
            for(int j=0;j<=n;j++)
                P[i][j]+=P[0][j]*(-a[i]/d0);
        //debug();
        //化简除第一行外每行
        for(int i=1;i<n;i++)
        {
            LL dd=gcd(P[i][1],P[i][2]);
            if(dd<0) dd=-dd;
            for(int j=3;j<=n;j++)
            {
                dd=gcd(dd,P[i][j]);
                if(dd<0) dd=-dd;
            }
            if(dd!=1)
                for(int j=1;j<=n;j++)
                    P[i][j]/=dd;
        }
        //debug();
        //解为[d/d0,u1,u2,...,un-1]*P
        //显然一组特解为[d/d0,0,0,...,0]*P
        cout<<"通解为 O（n^2）："<<endl;
        for(int j=1;j<=n;j++)
        {
            cout<<"x"<<j<<"= "<<c/d0*P[0][j]<<" + ";
            for(int i=1;i<n;i++)
                cout<<P[i][j]<<" *u"<<i<<((i==n-1)?"\n":" + ");
        }
        cout<<endl;
	}
	return 0;
}
