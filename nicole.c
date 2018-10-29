/*
kilcol064cb.c

This is a rewrite of the program through which Kiley found 
a 7-coll case.

The output is changed to avoid laborious copying and pasting
of data. In addition to values in human-readable format it
produces also a block of numbers that can be directly pasted
into another program of the kilcol series.

Rewrite of basic collisions program in 3D for 4 balls
Randomizes positions (in different octants) and velocities.
Takes threshold number of collisions as argument #3.
Version without complete display/storage of results
Kiley Sprigg, Christopher Wooten and Lukasz Pruski
August 2018
*/

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <float.h>

#include "Motherdefs.h"
#include "Motherstuff.c"

#define MAXBALLS 10
#define MAXCOLL 20
#define NOCOL -66666.66666

int numBalls = 4;

long double p[MAXBALLS][3], v[MAXBALLS][3];
long double po[MAXBALLS][3], ve[MAXBALLS][3];
int balArr[MAXCOLL][2];
long double timArr[MAXCOLL];
int fBalArr[MAXCOLL][2];
long double fTimArr[MAXCOLL];

long double at = 0.0;
long seed;
int threshhold;
int overThreshCtr = 0;

void initPosAndVel(void)
{	
	p[0][0] =  -1.931141240502 + 0.1*(1-2.0*Mother(&seed));
	v[0][0] =  0.898144606925 + 0.1*(1-2.0*Mother(&seed));
	p[0][1] =  1.920783800981 + 0.1*(1-2.0*Mother(&seed));
	v[0][1] =   -0.545101828380 + 0.1*(1-2.0*Mother(&seed));
	p[0][2] =   -1.416888265241 + 0.1*(1-2.0*Mother(&seed));
	v[0][2] =   0.668850271391 + 0.1*(1-2.0*Mother(&seed));
	p[1][0] =  1.767429006092 + 0.1*(1-2.0*Mother(&seed));
	v[1][0] =   -0.540790359590 + 0.1*(1-2.0*Mother(&seed));
	p[1][1] =   -1.570768042400 + 0.1*(1-2.0*Mother(&seed));
	v[1][1] =  0.876758880692 + 0.1*(1-2.0*Mother(&seed));
	p[1][2] =   -1.498820065148 + 0.1*(1-2.0*Mother(&seed));
	v[1][2] =   0.599644548370 + 0.1*(1-2.0*Mother(&seed));
	p[2][0] =   1.032276193544 + 0.1*(1-2.0*Mother(&seed));
	v[2][0] =  -0.545663650650 + 0.1*(1-2.0*Mother(&seed));
	p[2][1] =  -1.870707460682 + 0.1*(1-2.0*Mother(&seed));
	v[2][1] =   0.868440486369 + 0.1*(1-2.0*Mother(&seed));
	p[2][2] =   1.553810240718 + 0.1*(1-2.0*Mother(&seed));
	v[2][2] =   -0.446972650255 + 0.1*(1-2.0*Mother(&seed));
	p[3][0] =   -1.781169325351 + 0.1*(1-2.0*Mother(&seed));
	v[3][0] =  0.809106217420 + 0.1*(1-2.0*Mother(&seed));
	p[3][1] =  -1.159680655231 + 0.1*(1-2.0*Mother(&seed));
	v[3][1] =   0.162185098954 + 0.1*(1-2.0*Mother(&seed));
	p[3][2] =   1.978485523271 + 0.1*(1-2.0*Mother(&seed));
	v[3][2] =   0.411244294967 + 0.1*(1-2.0*Mother(&seed));

	for (int i = 0; i < 4; i++)
		for(int j = 0; j < 3; j++)
			v[i][j] *= 0.5;

	for(int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
		{
			po[i][j] = p[i][j];
			ve[i][j] = v[i][j];
		}

	for (int i = 0; i < MAXCOLL; i++)
	{
			balArr[i][0] = 0;
			balArr[i][1] = 0;
			fBalArr[i][0] = 0;
			fBalArr[i][1] = 0;
			timArr[i] = 0.0;
			fTimArr[i] = 0.0;
	}
}

long double findCollision(int i, int j)
{
	long double dx, dy, dz, dvx, dvy, dvz, a, b, c, D, sD, t1, t2, tCol;

	dx = p[j][0] - p[i][0];
	dy = p[j][1] - p[i][1];
	dz = p[j][2] - p[i][2];
	dvx = v[j][0] - v[i][0];
	dvy = v[j][1] - v[i][1];
	dvz = v[j][2] - v[i][2];
	a = dvx*dvx + dvy*dvy + dvz*dvz;
	b = 2.0*(dx*dvx + dy*dvy + dz*dvz);
	c = dx*dx + dy*dy + dz*dz -4.0;
	D = b*b - 4.0*a*c;
	if (D >= 0.0)
	{
		sD = sqrt(D);
		t1 = (-b - sD)/(2.0*a);
		t2 = (-b + sD)/(2.0*a);
		if ((t1 >= 0.0) && (t2 >= 0.0))
			tCol = t1;
		else if ((t1 <= 0.0) && (t2 <= 0.0))
			tCol = t2;
	}
	else tCol = NOCOL;

	return tCol;
}

void processCollision(int b1, int b2, long double tc)
{
	long double dx, dy, dz, vb1d, vb2d, nv; 

	/* Compute new positions */
	for (int i = 0; i < numBalls; i++)
		for (int j = 0; j < 3; j++)
			p[i][j] += v[i][j]*tc;
	
	/* Compute changed velocities */ 
	dx = p[b2][0] - p[b1][0];
	dy = p[b2][1] - p[b1][1];
	dz = p[b2][2] - p[b1][2];

	vb1d = dx*v[b1][0] + dy*v[b1][1] + dz*v[b1][2];
	vb2d = dx*v[b2][0] + dy*v[b2][1] + dz*v[b2][2];
	
	nv = (vb2d - vb1d)/4.0;

	v[b1][0] += dx*nv;
	v[b1][1] += dy*nv;
	v[b1][2] += dz*nv;
	v[b2][0] -= dx*nv;
	v[b2][1] -= dy*nv;
	v[b2][2] -= dz*nv;	

	at += tc;

	/* Display pos and vel after the collision */
/*
	printf("\nAfter the collision, current t = %10.4Lf\n", at);
	for (int i = 0; i < numBalls; i++)
	{
		printf("B#%2d x= %20.12Lf y= %20.12Lf z= %20.12Lf\n", 
			i, p[i][0], p[i][1], p[i][2]);
		printf("     vx=%20.12Lf vy=%20.12Lf vz=%20.12Lf\n\n", 
			v[i][0], v[i][1], v[i][2]);
	}
*/
}

int main(int argc, char *argv[])
{
	long double tij, maxNegTime, minPosTime;
	int more, cc, nPos, nNeg, b1, b2, totNegColl, totPosColl, 
		ctr, justDone1, justDone2;
	int numSim, maxNumColl = 0, sumNegColl = 0, sumPosColl = 0, sumAllColl = 0;

	seed = atof(argv[1]);
	numSim = atoi(argv[2]);
	threshhold = atoi(argv[3]);
	srand(seed);


	for (int k = 0; k < numSim; k++)  {
	more = 1;
	totNegColl = 0;
	totPosColl = 0;
	ctr = 0;
	at = 0.0;

	initPosAndVel();

	/* Display pos and vel before the collisions */
/*
	printf("\n\nAt the beginning:\n");
	for (int i = 0; i < numBalls; i++)
	{
		printf("B#%2d x= %20.12Lf y= %20.12Lf z= %20.12Lf\n", i, p[i][0], p[i][1], p[i][2]);
		printf("     vx=%20.12Lf vy=%20.12Lf vz=%20.12Lf\n\n", v[i][0], v[i][1], v[i][2]);
	}
*/
	justDone1 = -1;
	justDone2 = -1;
	while (more)   
	{
		cc = 0;
		nPos = 0;
		nNeg = 0;

		/* Check all potential collisions */
/*
		printf("\nChecking all potential collisions:\n");
*/
		for (int i = 0; i < numBalls; i++)
			for (int j = i+1; j < numBalls; j++)
			{
				if ((i != justDone1) || (j != justDone2))
				{
					tij = findCollision(i, j);
					if (tij > 0.0)
						nPos++;
					else if ((tij < 0.0) && (tij != NOCOL))
						nNeg++;
					balArr[cc][0] = i;
					balArr[cc][1] = j;
					timArr[cc] = tij;
					cc++;
/*
					printf("i=%2d j=%2d t=%20.12Lf\n", i, j, tij);
*/
				}
			}
/*
		printf("num pos coll =%2d num neg coll =%2d\n", nPos, nNeg);
*/
		/* Process negative collisions */
		if (nNeg > 0)
		{
/*
			printf("\nProcessing collisions in negative time\n");
*/

			for (int i = 0; i < numBalls; i++)
				for (int j = 0; j < 3; j++)
				{
					p[i][j] = po[i][j];
					v[i][j] = ve[i][j];
				}

			maxNegTime = -LDBL_MAX;
			for (int i = 0; i < cc; i++)
				if ((timArr[i] < 0.0) && (timArr[i] != NOCOL))
					if (timArr[i] > maxNegTime) 
					{
						maxNegTime = timArr[i]; 
						b1 = balArr[i][0];
						b2 = balArr[i][1];
					}
/*
			printf("\nmax neg t=%20.12Lf for b1=%2d b2=%2d\n", maxNegTime, b1, b2);
*/	
			processCollision(b1, b2, maxNegTime);
			fBalArr[ctr][0] = b1;
			fBalArr[ctr][1] = b2;
			fTimArr[ctr] = at;
			justDone1 = b1;
			justDone2 = b2;
			totNegColl++;
			ctr++;
		}
		else
		{
			more = 0;
		}
	}

	more = 1;
	at = 0.0;
	for (int i = 0; i < numBalls; i++)
		for (int j = 0; j < 3; j++)
		{
			p[i][j] = po[i][j];
			v[i][j] = ve[i][j];
		}

	/* Process collisions in positive time */
/*
	printf("\nProcessing collisions in positive time\n");
*/
	justDone1 = -1;
	justDone2 = -1;
	while (more)
	{
		cc = 0;
		nPos = 0;
		nNeg = 0;

		/* Check all potential collisions */
/*
		printf("\nChecking all potential collisions:\n");
*/
		for (int i = 0; i < numBalls; i++)
			for (int j = i+1; j < numBalls; j++)
			{
				if ((i != justDone1) || (j != justDone2))
				{
					tij = findCollision(i, j);
					if (tij > 0.0)
						nPos++;
					balArr[cc][0] = i;
					balArr[cc][1] = j;
					timArr[cc] = tij;
					cc++;
/*
					printf("i=%2d j=%2d t=%20.12Lf\n", i, j, tij);
*/
				}
			}
/*
		printf("num pos coll =%2d\n", nPos);
*/
		if (nPos > 0)
		{
			minPosTime = LDBL_MAX;
			for (int i = 0; i < cc; i++)
				if (timArr[i] > 0.0)
					if (timArr[i] < minPosTime) 
					{
						minPosTime = timArr[i]; 
						b1 = balArr[i][0];
						b2 = balArr[i][1];
					}
/*
			printf("\nmin pos t=%20.12Lf for b1=%2d b2=%2d\n", minPosTime, b1, b2);
*/	
			processCollision(b1, b2, minPosTime);
			fBalArr[ctr][0] = b1;
			fBalArr[ctr][1] = b2;
			fTimArr[ctr] = at;
			justDone1 = b1;
			justDone2 = b2;
			totPosColl++;
			ctr++;
		}
		else
		{
			more = 0;
		}
	}

	if (ctr > threshhold)
	{
		overThreshCtr++;
		printf("\n\nAt the beginning:\n");
		for (int i = 0; i < numBalls; i++)
		{
			printf("B#%2d x= %20.12Lf y= %20.12Lf z= %20.12Lf\n", i, po[i][0], po[i][1], po[i][2]);
			printf("     vx=%20.12Lf vy=%20.12Lf vz=%20.12Lf\n\n", ve[i][0], ve[i][1], ve[i][2]);
		}
		
		/* Print in suitable format */
		for (int i = 0; i < numBalls; i++)
			for (int j = 0; j < 3; j++)
			{
				printf("p[%1d][%1d] = %20.16Lf;\n", i, j, po[i][j]);
				printf("v[%1d][%1d] = %20.16Lf;\n", i, j, ve[i][j]);
			}

		printf("\nFINAL RESULTS:\n");
		printf("# of neg coll: %2d # of pos coll: %2d\n", totNegColl, totPosColl);
		for (int i = 0; i < ctr; i++)
		{
			printf("#%2d with #%2d at %20.12Lf\n", fBalArr[i][0], fBalArr[i][1],
				fTimArr[i]);
		}
	}

	if (ctr > maxNumColl)
		maxNumColl = ctr;
	sumAllColl += ctr;
	sumNegColl += totNegColl;
	sumPosColl += totPosColl;

	} // End of the simulation loop  for k

	printf("\n\nmax=%2d suNe=%6d suPo=%6d suAl=%6d ovrThrCtr=%6d\n",
		maxNumColl, sumNegColl, sumPosColl, sumAllColl, overThreshCtr);

	return 0;
}