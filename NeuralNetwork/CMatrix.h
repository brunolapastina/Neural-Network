#pragma once

class CMatrix
{
public:
	CMatrix();
	CMatrix( CMatrix& objOperand );
	CMatrix( int iLines, int iColumns );
	CMatrix( int iLines, int iColumns, double dInitialValue );

	~CMatrix();

	void Print();

	CMatrix& operator=(CMatrix& objOperand);

	CMatrix& operator+=(const double& dValue);
	CMatrix& operator+ (const double& dValue);
	CMatrix& operator-=(const double& dValue);
	CMatrix& operator- (const double& dValue);
	CMatrix& operator*=(const double& dValue);
	CMatrix& operator* (const double& dValue);

	CMatrix& operator+=(const CMatrix& objOperand);
	CMatrix& operator+ (const CMatrix& objOperand);
	CMatrix& operator-=(const CMatrix& objOperand);
	CMatrix& operator- (const CMatrix& objOperand);
	
	CMatrix operator*=(const CMatrix& objOperand);
	CMatrix operator* (const CMatrix& objOperand);

	double& operator()(unsigned i, unsigned j) { return m_mdData[i][j]; }

	int Rows() { return m_iLines; }
	int Columns() { return m_iColumns; }

private:
	double**	m_mdData;
	int	m_iLines;
	int	m_iColumns;
};