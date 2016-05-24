#include "CMatrix.h"
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>

CMatrix::CMatrix() : m_iLines(0),
					 m_iColumns(0),
					 m_mdData(NULL)
{
}

CMatrix::CMatrix(CMatrix& objOperand) : m_iLines(objOperand.m_iLines),
										m_iColumns(objOperand.m_iColumns),
										m_mdData(NULL)
{
	m_mdData = new double*[m_iLines];
	for (int i = 0; i < m_iLines; ++i)
	{
		m_mdData[i] = new double[m_iColumns];
		for (int j = 0; j < m_iColumns; ++j)
		{
			m_mdData[i][j] = objOperand.m_mdData[i][j];
		}
	}
}

CMatrix::CMatrix( int iLines, int iColumns ) :	m_iLines(iLines),
												m_iColumns(iColumns),
												m_mdData(NULL)
{
	m_mdData = new double*[m_iLines];
	for (int i = 0; i < m_iLines; ++i)
	{
		m_mdData[i] = new double[m_iColumns];
	}
}

CMatrix::CMatrix(int iLines, int iColumns, double dInitialValue ) : m_iLines(iLines),
																	m_iColumns(iColumns),
																	m_mdData(NULL)
{
	m_mdData = new double*[m_iLines];
	for (int i = 0; i < m_iLines; ++i)
	{
		m_mdData[i] = new double[m_iColumns];
		for (int j = 0; j < m_iColumns; ++j)
		{
			m_mdData[i][j] = dInitialValue;
		}
	}
}

CMatrix::~CMatrix()
{
	if (NULL != m_mdData)
	{
		for (int i = 0; i < m_iLines; ++i)
		{
			if (NULL != m_mdData[i])
			{
				delete [] m_mdData[i];
				m_mdData[i] = NULL;
			}
		}
		delete [] m_mdData;
		m_mdData = NULL;
	}
}

void CMatrix::Print()
{
	for (int i = 0; i < m_iLines; ++i)
	{
		printf("\t| ");
		for (int j = 0; j < m_iColumns; ++j)
		{
			printf("%3f ", m_mdData[i][j]);
		}
		printf("|\n");
	}
}

CMatrix& CMatrix::operator=(CMatrix& objOperand)
{
	if (NULL != m_mdData)
	{
		for (int i = 0; i < m_iLines; ++i)
		{
			if (NULL != m_mdData[i])
			{
				delete[] m_mdData[i];
				m_mdData[i] = NULL;
			}
		}
		delete[] m_mdData;
	}

	m_iLines = objOperand.m_iLines;
	m_iColumns = objOperand.m_iColumns;

	m_mdData = objOperand.m_mdData;

	objOperand.m_mdData = NULL;	// We´ve just stolen the clas variable

	return *this;
}

CMatrix& CMatrix::operator+=(const double& dValue)
{
	for (int i = 0; i < m_iLines; ++i)
		for (int j = 0; j < m_iColumns; ++j)
			m_mdData[i][j] += dValue;

	return *this;
}

CMatrix& CMatrix::operator+(const double& dValue)
{
	for (int i = 0; i < m_iLines; ++i)
		for (int j = 0; j < m_iColumns; ++j)
			m_mdData[i][j] += dValue;

	return *this;
}

CMatrix& CMatrix::operator-=(const double& dValue)
{
	for (int i = 0; i < m_iLines; ++i)
		for (int j = 0; j < m_iColumns; ++j)
			m_mdData[i][j] -= dValue;

	return *this;
}

CMatrix& CMatrix::operator-(const double& iValue)
{
	for (int i = 0; i < m_iLines; ++i)
		for (int j = 0; j < m_iColumns; ++j)
			m_mdData[i][j] -= iValue;

	return *this;
}

CMatrix& CMatrix::operator*=(const double& dValue)
{
	for (int i = 0; i < m_iLines; ++i)
		for (int j = 0; j < m_iColumns; ++j)
			m_mdData[i][j] *= dValue;

	return *this;
}

CMatrix& CMatrix::operator*(const double& dValue)
{
	for (int i = 0; i < m_iLines; ++i)
		for (int j = 0; j < m_iColumns; ++j)
			m_mdData[i][j] *= dValue;

	return *this;
}

CMatrix& CMatrix::operator+=(const CMatrix& objOperand)
{
	if ((m_iLines != objOperand.m_iLines) || (m_iColumns != objOperand.m_iColumns))
	{
		throw std::invalid_argument("Invalid dimensions");
	}

	for (int i = 0; i < m_iLines; ++i)
		for (int j = 0; j < m_iColumns; ++j)
			m_mdData[i][j] += objOperand.m_mdData[i][j];

	return *this;
}

CMatrix& CMatrix::operator+(const CMatrix& objOperand)
{
	if ((m_iLines != objOperand.m_iLines) || (m_iColumns != objOperand.m_iColumns))
	{
		throw std::invalid_argument("Invalid dimensions");
	}

	for (int i = 0; i < m_iLines; ++i)
		for (int j = 0; j < m_iColumns; ++j)
			m_mdData[i][j] += objOperand.m_mdData[i][j];

	return *this;
}

CMatrix& CMatrix::operator-=(const CMatrix& objOperand)
{
	if ((m_iLines != objOperand.m_iLines) || (m_iColumns != objOperand.m_iColumns))
	{
		throw std::invalid_argument("Invalid dimensions");
	}

	for (int i = 0; i < m_iLines; ++i)
		for (int j = 0; j < m_iColumns; ++j)
			m_mdData[i][j] -= objOperand.m_mdData[i][j];

	return *this;
}

CMatrix& CMatrix::operator-(const CMatrix& objOperand)
{
	if ((m_iLines != objOperand.m_iLines) || (m_iColumns != objOperand.m_iColumns))
	{
		throw std::invalid_argument("Invalid dimensions");
	}

	for (int i = 0; i < m_iLines; ++i)
		for (int j = 0; j < m_iColumns; ++j)
			m_mdData[i][j] -= objOperand.m_mdData[i][j];

	return *this;
}

CMatrix CMatrix::operator*=(const CMatrix& objOperand)
{
	if (m_iColumns != objOperand.m_iLines )
	{
		throw std::invalid_argument("Invalid dimensions");
	}

	double** m_mdCurrentData = m_mdData;

	m_mdData = new double*[m_iLines];
	for (int i = 0; i < m_iLines; ++i)
	{
		m_mdData[i] = new double[objOperand.m_iColumns];
		for (int j = 0; j < objOperand.m_iColumns; ++j)
		{
			m_mdData[i][j] = 0;
			for (int z = 0; z < m_iColumns; ++z)
			{
				m_mdData[i][j] += m_mdCurrentData[i][z] * objOperand.m_mdData[z][j];
			}
		}
	}

	for (int i = 0; i < m_iLines; ++i)
	{
		delete[] m_mdCurrentData[i];
		m_mdCurrentData[i] = NULL;
	}
	delete[] m_mdCurrentData;

	m_iColumns = objOperand.m_iColumns;

	return *this;
}

CMatrix CMatrix::operator*(const CMatrix& objOperand)
{
	if (m_iColumns != objOperand.m_iLines)
	{
		throw std::invalid_argument("Invalid dimensions");
	}

	double** m_mdCurrentData = m_mdData;

	m_mdData = new double*[m_iLines];
	for (int i = 0; i < m_iLines; ++i)
	{
		m_mdData[i] = new double[objOperand.m_iColumns];
		for (int j = 0; j < objOperand.m_iColumns; ++j)
		{
			m_mdData[i][j] = 0;
			for (int z = 0; z < m_iColumns; ++z)
			{
				m_mdData[i][j] += m_mdCurrentData[i][z] * objOperand.m_mdData[z][j];
			}
		}
	}

	for (int i = 0; i < m_iLines; ++i)
	{
		delete[] m_mdCurrentData[i];
		m_mdCurrentData[i] = NULL;
	}
	delete[] m_mdCurrentData;

	m_iColumns = objOperand.m_iColumns;

	return *this;
}