// Classes for interface GeoSys - ECLIPSE
#include <vector>
//#define MAX_ZEILEN 512

class CRFProcess;
#include "msh_faces.h"
#include "rf_pcs.h"

typedef struct
{
	double Number;
	int Exponent;
}typeExponentialNumber;
// KB: neue Struktur zum Einlesen der Brunnendatei, siehe CECLIPSEData::ReadWellData()
struct structWell   /* deklariert den Strukturtyp well, wird beim Einlesen von .well-File neu aufgerufen */
{std::string name;
 std::vector <double> time;
 std::vector <std::string> rate;
 std::vector <std::string> limit;
 std::vector < std::vector <std::string> > comp_mix;	// WTP 01/2014: modifications needed in order to deal with multiple components
 std::vector <std::string> phase;
 std::vector <std::string> open_flag;
 std::vector <std::string> control_mode;
	//std::string dummy_rate;
	//std::string dummy_zeile;
	//std::string rest_zeile;
};

class CECLIPSEBlock
{
public:
	long index;
	long row;
	long column;
	long layer;
	std::vector <double> x_coordinates;
	std::vector <double> y_coordinates;
	std::vector <double> z_coordinates;
	double x_barycentre;
	double y_barycentre;
	double z_barycentre;
	int active;

	std::vector<long> MshNodeindex;//BW:record the index of Eclipse cell used for constructing ogs mesh, non double index
	std::vector<long> Nodeindex; //BW:read the nodeindex generated by mesh convertor
	std::vector <long> FaceIndex;//BW:store the face index
	int correspondingelenum;
	std::vector<long> correspondingeleindex;//BW:read the corresponding index and number
	int decomposetype;//decomposition type for a hexahe

	double volume;
	std::vector <long> connected_faces;

	std::vector <long> NeighbourElement;
	std::vector <long> ConnectedBoundaryCondition;

	CECLIPSEBlock(long Nodelength, long Facelength);
	~CECLIPSEBlock();

	//void CalcBarycentre(void);
	void CalculateFaceCentres(void);
};

class CReadTextfiles_ECL
{
public:
	std::vector <std::string> Data;
	std::vector <std::vector <std::string> > Data_separated;
	long NumberOfRows;
	std::vector <std::string> SplittedString;
	std::vector <std::string> Header;

	CReadTextfiles_ECL(); //Konstruktor
	~CReadTextfiles_ECL(); //Desturktor

	bool Read_Text(std::string Filename);

	void SplitStrings(const std::string str, std::string delimiter);

	bool Read_SeparatedText(std::string Filename, std::string delimiter);

	//vector <string> Header;
	//   vector <string> Data;
	//   long NumberOfRows;

	//CReadTextfiles();		//Konstruktor
	//~CReadTextfiles();		//Desturktor

	//bool Read_Text(std::string Filename);
};

class CWriteTextfiles_ECL
{
public:
	CWriteTextfiles_ECL(); //Konstruktor
	~CWriteTextfiles_ECL(); //Desturktor

	void Write_Text(std::string Filename, std::vector<std::string> Text);
};

class CPointData_ECL
{
public:
	double x;
	double y;
	double z;
	//double Flow[3];
	double pressure;
	double temperature;
	//double Gas_dissolved;	// WTP 04/2014
	//double CO2inLiquid;
	//double GasinLiquid;
	//double NaClinLiquid;
	//double deltaDIC;
	//double deltaSat;
    double deltaSatGas;
    double deltaSatOil;
	double deltaPress1;
	double deltaPress2;
    double deltaPress3;
	// WTP 04/2014: instead of CO2inLiquid we need vectors to accomodate various compositional settings
    // Variables to store nodal component concentation (interpolated values)
	//double VaporComponentMassFraction;
	std::vector <double> CompConcInWater_Nodes;
	std::vector <double> CompConcInGas_Nodes;
    std::vector <double> mCompInGas_Nodes;
    std::vector <double> CompConcInOil_Nodes;
	std::vector <double> phase_pressure;
	std::vector <double> phase_saturation;
	std::vector <double> phase_reservoir_density;
	std::vector <double> phase_surface_density;
    std::vector <double> phase_viscosity;
	std::vector <double> phase_fvf;
	std::vector <double> delta_CompConcInWater_Nodes; // exchange for deltaDIC
	std::vector <double> delta_CompConcInGas_Nodes;
    std::vector <double> delta_CompConcInOil_Nodes;
	std::vector <std::vector <double> > q;
	std::vector <size_t> connect_block;	//Record all connected Blocks

	CPointData_ECL()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	CPointData_ECL(double const* const pnt)
	{
		x = pnt[0];
		y = pnt[1];
		z = pnt[2];
	}
	~CPointData_ECL() {}
};

class CBoundaryConditions
{
public:
	int index;
	int number;
	long connected_element;
	std::string boundary_position;
	double value[4];
	CBoundaryConditions()
	{
		index = -1;
		number = -1;
		connected_element = -1;
		boundary_position = "";
	}
	~CBoundaryConditions() {}
};

class CECLIPSEData
{
public:
	std::string pathECLProject; // WTP 04/2014
	std::string pathECLFolder;
	std::string nameECLProject;
	std::string pathECLFFile;
	std::string pathECLWellFile;
	long elements;
	long rows;
	long columns;
	long layers;
	//WTP long times;
	int timestep_adjust_initial; // WTP useful if the (first) eclipse run is already a restart of some other simulation
	int timestep_adjust_iteration; //KB1115 is needed for iterations between OGS and Eclipse for coupled HM simulations
	int timestep_adjust_iteration_tot; //KB1115 counts total iterations of simulation
	int numberOutputParameters;
	int activeCells;
	// Adjust for counting the number of Eclipse grid appears in OGS mesh,BW
	int eclgridelenum; 
	// Information about the time when '*.list'/'*.neightbours'/'*.bfaces' files are generated, BW
	long IdentificationNumber;
	// Total Number of Faces attached to ECLIPSE Block
	long FlowFaceNum;
	//Index Matrix according to I,J,K of Eclipse Grid
	std::vector<std::vector<std::vector<long> > > IndexArray;

	bool Radialmodell;
	bool Radial_J;
	bool Radial_I;
	bool RadialModellIpos;
	bool RadialModellJpos;
	double Molweight_CO2; // [g/mol]
	double Molweight_H2O; // [g/mol]
	double Molweight_NaCl; // [g/mol]
	double SurfaceDensity_Gas_E100;
	// WTP double SurfaceCO2Density;
	double surface_pressure;	// WTP 04/2014
	double surface_temperature;
	bool E100;
	bool option_CO2STORE;
	bool phase_shift_flag;
	bool reservoir_conditions; //WTP bool flag to request surface conditions (default=reservoir cond)
	//WTP double sumCO2removed;
	int ProcessIndex_CO2inLiquid;
	int ProcessIndex_GasinLiquid;
	int ProcessIndex_CO2inGas; //KB 
	double actual_time;
	bool Windows_System;
	bool existWells;
	bool UsePrecalculatedFiles;
	bool UseSaveEclipseDataFiles;
	int verbosity;

	bool UseEclrun; // WTP 02/15
    
	bool TempIncludeFile;	// WTP bool flag to select temperature exchange with ecl and geosys
	//WTP std::string dissolved_co2_pcs_name_ECL; // Keyword DISSOLVED_CO2_PCS_NAME, Name of MASS_TRANSPORT Process which is used to store total dissolved CO2 from ECLIPSE
	//WTP std::string dissolved_co2_ingas_pcs_name_ECL; // KB
	bool Water_phase_exists;
    bool Oil_phase_exists;
    bool Gas_phase_exists;
	// WTP keywords associated with component coupling OGS-ECLIPSE
	int no_comps_ECL;
	bool dissolved_comps;
    // WTP keywords to ensure single-function calls
    bool ReadEclipseGrid_called;
    bool ReadCorrespondingList_called;
    bool DetermineNeighbourElements_called;
    bool CompareElementsGeosysEclipse_called;
    bool CreateFaces_called;
    bool ConnectFacesToElements_called;
    bool ReadWellData_called;
        

	std::vector <std::string> vec_cnames_ecl;
	//	std::vector <std::string> vec_rptrst_keys;
	std::vector <double> vec_ecl_mw_comps;

    // variable to store int flags for all components
	std::vector <std::vector<int> > vec_components_ECL_OGS;
	//std::vector <std::vector<double> > vec_RS_like_values_elements;
    // variables to store the concentrations of all comps in each phase (element wise)
	std::vector <std::vector<double> > vec_CompConc_Water_elements;
	std::vector <std::vector<double> > vec_CompConc_Gas_elements;
	std::vector <std::vector<double> > vec_CompConc_Oil_elements;
    std::vector <double> vec_RS_elements;
    std::vector <std::vector<double> > vec_Comp_MLSC_elements;
    std::vector <std::vector<double> > vec_mComp_gas_elements;
    std::vector <double> vec_SGAS;
    std::vector<double> vec_SOIL;
    std::vector<double> vec_SWAT;
	std::vector<double> vec_PRESS1;//KB
    std::vector<double> vec_PRESS2;
    std::vector<double> vec_PRESS3;
    //std::vector <std::vector<double> > vec_Comp_MLSC_elements;
    //std::vector <double> vec_RS_elements;
	//std::vector <std::vector<double> > vec_CompConc_like_values_elements;
	std::vector <double> vec_SurfaceDensity_Gas_Element;
	std::vector <std::vector<std::string> > vec_components_ECL_OGS_pcs_names;

    // variables to store ogs process name and index
    /*typedef std::vector<std::pair<std::string, int> > VecOfStringIntPairs;
	VecOfStringIntPairs vec_OGS_process_index_comps_water;
    VecOfStringIntPairs vec_OGS_process_index_comps_gas;
	VecOfStringIntPairs vec_OGS_process_index_comps_oil;*/
    //typedef std::vector<std::pair<std::string, int> > VecOfStringIntPairs;
	std::vector<std::pair<std::string, int> > vec_OGS_process_index_comps_water;
    std::vector<std::pair<std::string, int> > vec_OGS_process_index_comps_gas;
	std::vector<std::pair<std::string, int> > vec_OGS_process_index_comps_oil;

	std::vector <CECLIPSEBlock*> eclgrid;
	std::vector <std::string> SplittedString;
	std::vector <std::string> Variables;
	std::vector <CFaces*> faces;
	std::vector <CPointData_ECL*> NodeData;
	std::vector <CBoundaryConditions*> BC;
	std::vector <structWell*> ecl_well;
	
	std::vector <std::string> ECL_schedule_section;
	std::vector <std::string> ECL_keyword_messages;
	std::vector <std::string> ECL_keyword_rptrst;
	std::vector <std::string> ECL_keyword_welspecs;
	std::vector <std::string> ECL_keyword_compdat;
	std::vector <std::string> ECL_keyword_wellstre;
	std::vector <std::string> ECL_keyword_winjgas;
	std::vector <std::string> ECL_keyword_wconinje;
	std::vector <std::string> ECL_keyword_zippy2;
	std::vector <std::string> ECL_keyword_tuning;
	std::vector <std::string> ECL_keyword_tstep;
	std::vector <std::string> ECL_keyword_wconprod;

	double** Data; //array of points, times and variables
	std::vector <std::string> WellRates; // KB, abspeichern der neuen Raten f�r einen bestimmten Zeitschritt
	
	process::CRFProcessDeformation* dm_pcs; //KB0815 in der RunEclipse() gesetzt
	//CFiniteElementStd* fem; //KB0714
	//CFiniteElementVec* fem_dm; //KB0714

	std::vector <long> output_x;
	std::vector <long> output_y;
	std::vector <long> output_z;
	std::vector <long> output_time;
	std::vector <long> CorrespondingEclipseElement;
	std::vector <long> CorrespondingGeosysElement;
	std::vector <std::string> Phases;
	std::vector <std::string> Components;
	// WTP 04/2104: vectors to save variable index
	//std::vector <long> vec_xmf_indicies;
	//std::vector <long> vec_ymf_indicies;
	//std::vector <long> vec_xfw_indicies;
	//std::vector <long> vec_rsw_indicies;
	//std::vector <long> vec_yfw_indicies;
	//std::vector <long> vec_mlsc_indicies;
	//std::vector <long> vec_gas_comps_inLiquid_indicies;
    
    // variables to store index of keyword in data file
	// std::vector <long> vec_comps_water_ECL_indicies;
    long comp_water_ECL_indicies;  // only used for E100
    std::vector <long> vec_comps_total_indices;
	std::vector <long> vec_comps_gas_ECL_indicies;
	std::vector <long> vec_comps_oil_ECL_indicies;
    std::vector <long> vec_ecl_comp_indicies;
	
    std::vector <long> vec_floI_indicies;
	std::vector <long> vec_floJ_indicies;
	std::vector <long> vec_floK_indicies;

	long a [8][2]; //2D Array um Keywords abzuspeichern
	std::vector <bool> eclipse_ele_active_flag; // CB
	bool PoroPermIncludeFile;
	CECLIPSEData();
	~CECLIPSEData();

	int GetVariableIndex(std::string Variablename);

	void SplitStrings(const std::string str,
	                  std::string delimiter);

	double Round(double Number, int Decimalplaces);

	typeExponentialNumber RoundEXP(double Number, int Decimalplaces);

	std::string AddZero(double Number, int Places, bool before);
	
	bool SetFilenamesAndPaths(CRFProcess* m_pcs, long Timestep);    // WTP 04/2014

	bool CheckIfFileExists(std::string strFilename);

	bool ReplaceASectionInFile(std::string Filename,
	                           std::string Keyword,
	                           std::vector <std::string> Data,
	                           bool CheckLengthOfSection);
							   
	bool ReplaceASectionInData(CReadTextfiles_ECL* eclFFile,		// WTP 04/2014
	                           std::string Keyword,
	                           std::vector <std::string> Data,
	                           bool CheckLengthOfSection);

	bool WriteIncludeFile(std::string Filename, std::string Keyword, std::vector <std::string> Data, bool append); //CB

	bool ReplaceWellRate(CReadTextfiles_ECL* eclDataFile);

	int WriteDataBackToEclipse(CReadTextfiles_ECL* eclFFile, CReadTextfiles_ECL* eclDataFile, CRFProcess* m_pcs, std::string projectname);

    bool CalculateDeltaGeoSysECL(CRFProcess* m_pcs);

	bool InterpolateDeltaGeoSysECL(CRFProcess* m_pcs);

	void ExecuteEclipse(CReadTextfiles_ECL* eclDataFile, CReadTextfiles_ECL* eclFFile, long Timestep, CRFProcess* m_pcs);

	void ReadEclipseGrid(std::string Filename);

	void DetermineNeighbourElements(std::string Filename);

	bool ReadBoundaryData(int index_boundary, std::vector <std::string> Data);

	int AnalyzeDataFromInputFile(CReadTextfiles_ECL* eclDataFile, CRFProcess* m_pcs);

	void InitializeProject(CRFProcess* m_pcs, CReadTextfiles_ECL* eclDataFile);

	void GeneralBookkeeping(void);

	//WTP void GeneralBookkeeping(CRFProcess* m_pcs);

	bool ReadPositionBoundaryCondition(std::string Filename);

	bool CorrespondingElements(void);

	bool CompareElementsGeosysEclipse(void);
	
	// TF commented out method since we have already functions for computing the distance
	// between points
	//double CalculateDistanceBetween2Points(double Point1[3], double Point2[3]);

	bool CreateFaces(std::string Filename);
	
	bool ConnectFacesToElements(void);

	//bool MakeNodeVector(CRFProcess *m_pcs, std::string path, int timestep, int phase_index);
	bool MakeNodeVector(void);

	void ReadEclipseData(std::string Pathname);

	//void CalculateRSfromMassFraction_E300();
	void ConvertEclipseDataToUniformUnits(CRFProcess* m_pcs, long Timestep);

    void GetHeatDataFromOGS(int idx);

	bool GetFlowForFaces(int phase_index);

	bool GetVelForFaces(void);

	bool CalcBlockBudget(int phase_index);

	void InterpolateDataFromFacesToNodes(long ele_nr,
	                                     double* n_vel_x,
	                                     double* n_vel_y,
	                                     double* n_vel_z,
	                                     int phase_index);

	void InterpolateDataFromBlocksToNodes(CRFProcess* m_pcs, std::string path, int phase_index, long Timestep);

	void InterpolateGeosysVelocitiesToNodes(CRFProcess* m_pcs, double* vel_nod, long node);

	void WriteDataToGeoSys(CRFProcess* m_pcs, std::string folder);
	
	void SaveEclipseDataFile(long Timestep, CRFProcess* m_pcs); // WTP function to save a copy of the .data file 

	bool CleanUpEclipseFiles(std::string folder, std::string projectname, long Timestep, CRFProcess* m_pcs);

	bool SaveEclipseInputFiles(std::string folder, std::string projectname);

	int RunEclipse(long Timestep, CRFProcess* m_pcs);

	void ReadWellData(std::string Filename_Wells);

	void WriteOutput_2DSection(long Timestep);

    std::vector<std::vector<double> > GetMolComponentsSorted(std::vector<std::vector<double> > vec_mComps);
	
	void ReadCorrespondingList(std::string Filename);

	void FaceGenerator(long elccellindex, int facenodeindex[], int category);

	std::string GetECLFlowVariableName(int phase, int direction);
};
