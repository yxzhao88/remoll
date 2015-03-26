#ifndef remollIO_HH
#define remollIO_HH

#include "vector"
#include "TROOT.h"
#include "TObject.h"
#include "G4Run.hh"
#include "remolltypes.hh"

#include "G4String.hh"

class TFile;
class TTree;

class remollGenericDetectorHit;
class remollGenericDetectorSum;
class remollEvent;

#include <xercesc/dom/DOMElement.hpp>

#define __FILENAMELEN 255

// Units for output
#define __E_UNIT GeV
#define __L_UNIT m
#define __T_UNIT ns
#define __ANG_UNIT rad
#define __ASYMM_SCALE 1e-9 // ppb

class remollIO {
    public:
	 remollIO();
	~remollIO();

	void SetFilename(G4String  fn);
	G4String GetFilename(){return fFilename;}

	void FillTree();
	void Flush();
	void WriteTree();

	void WriteRun();

	void InitializeTree();

	void GrabGDMLFiles( G4String fn );

    private:
	TFile *fFile;
	TTree *fTree;

	char fFilename[__FILENAMELEN];

	std::vector<G4String>       fGDMLFileNames;

	void SearchGDMLforFiles(G4String );
	void TraverseChildren(  xercesc::DOMElement * );

	//  Interfaces and buffers to the tree
	//  This is potentially going to get very long...

	// Event data
    public:
	void SetEventData(remollEvent *);
    private:
	Int_t fNEvPart;

	Double_t fEvRate;
	Double_t fEvEffXS;
	Double_t fEvAsym;
	Double_t fEvmAsym;
	Double_t fEvBeamP;
	Double_t fEvQ2;
	Double_t fEvW2;
	Double_t fEvThCoM;

	Double_t fBmX;
	Double_t fBmY;
	Double_t fBmZ;
	Double_t fBmdX;
	Double_t fBmdY;
	Double_t fBmdZ;

	std::vector<Int_t> *fEvPID;

	std::vector<Double_t> *fEvPart_X;
	std::vector<Double_t> *fEvPart_Y;
	std::vector<Double_t> *fEvPart_Z;
	std::vector<Double_t> *fEvPart_Px;
	std::vector<Double_t> *fEvPart_Py;
	std::vector<Double_t> *fEvPart_Pz;
	std::vector<Double_t> *fEvPart_Th;
	std::vector<Double_t> *fEvPart_Ph;
	std::vector<Double_t> *fEvPart_P;
	std::vector<Double_t> *fEvPart_tPx;
	std::vector<Double_t> *fEvPart_tPy;
	std::vector<Double_t> *fEvPart_tPz;


	//  GenericDetectorHit
    public:
	void AddGenericDetectorHit(remollGenericDetectorHit *);
    private:
	Int_t fNGenDetHit;
	std::vector<Int_t> *fGenDetHit_det;
	std::vector<Int_t> *fGenDetHit_id;

	std::vector<Int_t> *fGenDetHit_trid;
	std::vector<Int_t> *fGenDetHit_pid;
	std::vector<Int_t> *fGenDetHit_gen;
	std::vector<Int_t> *fGenDetHit_mtrid;

	std::vector<Double_t> *fGenDetHit_X;
	std::vector<Double_t> *fGenDetHit_Y;
	std::vector<Double_t> *fGenDetHit_Z;
	std::vector<Double_t> *fGenDetHit_R;
	std::vector<Double_t> *fGenDetHit_Ph;

	std::vector<Double_t> *fGenDetHit_Px;
	std::vector<Double_t> *fGenDetHit_Py;
	std::vector<Double_t> *fGenDetHit_Pz;
	std::vector<Double_t> *fGenDetHit_P;
	std::vector<Double_t> *fGenDetHit_E;
	std::vector<Double_t> *fGenDetHit_M;

	std::vector<Double_t> *fGenDetHit_Vx;
	std::vector<Double_t> *fGenDetHit_Vy;
	std::vector<Double_t> *fGenDetHit_Vz;

	Int_t fCollCut;

	//  GenericDetectorSum
    public:
	void AddGenericDetectorSum(remollGenericDetectorSum *);
    private:
	Int_t fNGenDetSum;
	std::vector<Int_t> *fGenDetSum_det;
	std::vector<Int_t> *fGenDetSum_id;
	std::vector<Double_t> *fGenDetSum_edep;
};

#endif//remollIO_HH
