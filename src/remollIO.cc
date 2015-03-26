#include "remollIO.hh"

#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>

#include "G4ParticleDefinition.hh"

#include "remollGenericDetectorHit.hh"
#include "remollGenericDetectorSum.hh"
#include "remollEvent.hh"
#include "remollRun.hh"
#include "remollRunData.hh"
#include "remollBeamTarget.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNode.hpp>


remollIO::remollIO(){
    fTree = NULL;
    fFile = NULL;
    
    // Default filename
    strcpy(fFilename, "remollout.root");

    //  Set arrays to 0
    fNGenDetHit = 0;
    fNGenDetSum = 0;
    fCollCut = 1; // default

    fEvPID = new std::vector<Int_t>;
    fEvPart_X = new std::vector<Double_t>;
    fEvPart_Y = new std::vector<Double_t>;
    fEvPart_Z = new std::vector<Double_t>;
    fEvPart_Px = new std::vector<Double_t>;
    fEvPart_Py = new std::vector<Double_t>;
    fEvPart_Pz = new std::vector<Double_t>;
    fEvPart_Th = new std::vector<Double_t>;
    fEvPart_Ph = new std::vector<Double_t>;
    fEvPart_P = new std::vector<Double_t>;
    fEvPart_tPx = new std::vector<Double_t>;
    fEvPart_tPy = new std::vector<Double_t>;
    fEvPart_tPz = new std::vector<Double_t>;
    fGenDetHit_det = new std::vector<Int_t>;
    fGenDetHit_id = new std::vector<Int_t>;
    fGenDetHit_trid = new std::vector<Int_t>;
    fGenDetHit_pid = new std::vector<Int_t>;
    fGenDetHit_gen = new std::vector<Int_t>;
    fGenDetHit_mtrid = new std::vector<Int_t>;
    fGenDetHit_X = new std::vector<Double_t>;
    fGenDetHit_Y = new std::vector<Double_t>;
    fGenDetHit_Z = new std::vector<Double_t>;
    fGenDetHit_R = new std::vector<Double_t>;
    fGenDetHit_Ph = new std::vector<Double_t>;
    fGenDetHit_Px = new std::vector<Double_t>;
    fGenDetHit_Py = new std::vector<Double_t>;
    fGenDetHit_Pz = new std::vector<Double_t>;
    fGenDetHit_P = new std::vector<Double_t>;
    fGenDetHit_E = new std::vector<Double_t>;
    fGenDetHit_M = new std::vector<Double_t>;
    fGenDetHit_Vx = new std::vector<Double_t>;
    fGenDetHit_Vy = new std::vector<Double_t>;
    fGenDetHit_Vz = new std::vector<Double_t>;
    fGenDetSum_det = new std::vector<Int_t>;
    fGenDetSum_id = new std::vector<Int_t>;
    fGenDetSum_edep = new std::vector<Double_t>;

    InitializeTree();
    
    // To start the event Flush the data
    Flush();
}

remollIO::~remollIO(){
    if( fTree ){ delete fTree; }
    fTree = NULL;
    if( fFile ){ delete fFile; }
    fFile = NULL;

    // delete all the data vectors
   delete fEvPID;
   delete fEvPart_X;
   delete fEvPart_Y;
   delete fEvPart_Z;
   delete fEvPart_Px;
   delete fEvPart_Py;
   delete fEvPart_Pz;
   delete fEvPart_Th;
   delete fEvPart_Ph;
   delete fEvPart_P;
   delete fEvPart_tPx;
   delete fEvPart_tPy;
   delete fEvPart_tPz;
   delete fGenDetHit_det;
   delete fGenDetHit_id;
   delete fGenDetHit_trid;
   delete fGenDetHit_pid;
   delete fGenDetHit_gen;
   delete fGenDetHit_mtrid;
   delete fGenDetHit_X;
   delete fGenDetHit_Y;
   delete fGenDetHit_Z;
   delete fGenDetHit_R;
   delete fGenDetHit_Ph;
   delete fGenDetHit_Px;
   delete fGenDetHit_Py;
   delete fGenDetHit_Pz;
   delete fGenDetHit_P;
   delete fGenDetHit_E;
   delete fGenDetHit_M;
   delete fGenDetHit_Vx;
   delete fGenDetHit_Vy;
   delete fGenDetHit_Vz;
   delete fGenDetSum_det;
   delete fGenDetSum_id;
   delete fGenDetSum_edep;
}

void remollIO::SetFilename(G4String fn){
    G4cout << "Setting output file to " << fn << G4endl;
    strcpy(fFilename, fn.data());
}

void remollIO::InitializeTree(){
    if( fFile ){
	fFile->Close();
	delete fFile;
    }

    fFile = new TFile(fFilename, "RECREATE");

	//fFile already closed, no need to delete fTree (this makes valgrind happy :D)
    //if( fTree ){ delete fTree; }

    fTree = new TTree("T", "Geant4 Moller Simulation");

    // Event information
    fTree->Branch("rate",     &fEvRate,   "rate/D");
    fTree->Branch("ev.A",     &fEvAsym,   "ev.A/D");
    fTree->Branch("ev.Am",    &fEvmAsym,  "ev.Am/D");
    fTree->Branch("ev.xs",    &fEvEffXS,  "ev.xs/D");
    fTree->Branch("ev.Q2",    &fEvQ2,     "ev.Q2/D");
    fTree->Branch("ev.W2",    &fEvW2,     "ev.W2/D");
    fTree->Branch("ev.thcom", &fEvThCoM,  "ev.thcom/D");
    fTree->Branch("ev.beamp",  &fEvBeamP,   "ev.beamp/D");

    fTree->Branch("ev.npart", &fNEvPart,    "ev.npart/I");
    fTree->Branch("ev.pid",   &fEvPID);
    fTree->Branch("ev.vx",    &fEvPart_X);
    fTree->Branch("ev.vy",    &fEvPart_Y);
    fTree->Branch("ev.vz",    &fEvPart_Z);
    fTree->Branch("ev.p",     &fEvPart_P);
    fTree->Branch("ev.px",    &fEvPart_Px);
    fTree->Branch("ev.py",    &fEvPart_Py);
    fTree->Branch("ev.pz",    &fEvPart_Pz);
    fTree->Branch("ev.th",    &fEvPart_Th);
    fTree->Branch("ev.ph",    &fEvPart_Ph);
    fTree->Branch("ev.tpx",    &fEvPart_tPx);
    fTree->Branch("ev.tpy",    &fEvPart_tPy);
    fTree->Branch("ev.tpz",    &fEvPart_tPz);

    fTree->Branch("bm.x",    &fBmX,  "bm.x/D");
    fTree->Branch("bm.y",    &fBmY,  "bm.y/D");
    fTree->Branch("bm.z",    &fBmZ,  "bm.z/D");
    fTree->Branch("bm.dx",    &fBmdX,  "bm.dx/D");
    fTree->Branch("bm.dy",    &fBmdY,  "bm.dy/D");
    fTree->Branch("bm.dz",    &fBmdZ,  "bm.dz/D");

    // GenericDetectorHit
    fTree->Branch("hit.n",    &fNGenDetHit,     "hit.n/I");
    fTree->Branch("hit.det",  &fGenDetHit_det);
    fTree->Branch("hit.vid",  &fGenDetHit_id);

    fTree->Branch("hit.pid",  &fGenDetHit_pid);
    fTree->Branch("hit.trid", &fGenDetHit_trid);
    fTree->Branch("hit.mtrid",&fGenDetHit_mtrid);
    fTree->Branch("hit.gen",  &fGenDetHit_gen);

    fTree->Branch("hit.x",    &fGenDetHit_X);
    fTree->Branch("hit.y",    &fGenDetHit_Y);
    fTree->Branch("hit.z",    &fGenDetHit_Z);
    fTree->Branch("hit.r",    &fGenDetHit_R);
    fTree->Branch("hit.ph",   &fGenDetHit_Ph);

    fTree->Branch("hit.px",   &fGenDetHit_Px);
    fTree->Branch("hit.py",   &fGenDetHit_Py);
    fTree->Branch("hit.pz",   &fGenDetHit_Pz);

    fTree->Branch("hit.vx",   &fGenDetHit_Vx);
    fTree->Branch("hit.vy",   &fGenDetHit_Vy);
    fTree->Branch("hit.vz",   &fGenDetHit_Vz);

    fTree->Branch("hit.p",    &fGenDetHit_P);
    fTree->Branch("hit.e",    &fGenDetHit_E);
    fTree->Branch("hit.m",    &fGenDetHit_M);

    fTree->Branch("hit.colCut",    &fCollCut,     "hit.colCut/I");

    // GenericDetectorSum
    fTree->Branch("sum.n",    &fNGenDetSum,     "sum.n/I");
    fTree->Branch("sum.det",  &fGenDetSum_det);
    fTree->Branch("sum.vid",  &fGenDetSum_id);
    fTree->Branch("sum.edep", &fGenDetSum_edep);

    return;
}

void remollIO::FillTree(){
    if( !fTree ){ 
	fprintf(stderr, "Error %s: %s line %d - Trying to fill non-existant tree\n", __PRETTY_FUNCTION__, __FILE__, __LINE__ );
	return; 
    }

    fTree->Fill();
    fTree->GetCurrentFile();
}

void remollIO::Flush(){
    //  Set arrays to 0
    fNGenDetHit = 0;
    fNGenDetSum = 0;
    fCollCut = 1; // default
    
    // Now flush all vectors
    fEvPID->clear();
    
    fEvPart_X->clear();
	fEvPart_Y->clear();
	fEvPart_Z->clear();
	fEvPart_Px->clear();
	fEvPart_Py->clear();
	fEvPart_Pz->clear();
	fEvPart_Th->clear();
	fEvPart_Ph->clear();
	fEvPart_P->clear();
	fEvPart_tPx->clear();
	fEvPart_tPy->clear();
	fEvPart_tPz->clear();
	
	fGenDetHit_det->clear();
	fGenDetHit_id->clear();

	fGenDetHit_trid->clear();
	fGenDetHit_pid->clear();
	fGenDetHit_gen->clear();
	fGenDetHit_mtrid->clear();

	fGenDetHit_X->clear();
	fGenDetHit_Y->clear();
	fGenDetHit_Z->clear();
	fGenDetHit_R->clear();
	fGenDetHit_Ph->clear();

	fGenDetHit_Px->clear();
	fGenDetHit_Py->clear();
	fGenDetHit_Pz->clear();
	fGenDetHit_P->clear();
	fGenDetHit_E->clear();
	fGenDetHit_M->clear();

	fGenDetHit_Vx->clear();
	fGenDetHit_Vy->clear();
	fGenDetHit_Vz->clear();
	
	fGenDetSum_det->clear();
	fGenDetSum_id->clear();
	fGenDetSum_edep->clear();
}

void remollIO::WriteTree(){
    assert( fFile );
    assert( fTree );

    if( !fFile->IsOpen() ){
	G4cerr << "ERROR: " << __FILE__ << " line " << __LINE__ << ": TFile not open" << G4endl;
	exit(1);
    }

    G4cout << "Writing output to " << fFile->GetName() << "... ";

    fFile->cd();

    fTree->Write("T", TObject::kOverwrite);
    remollRun::GetRun()->GetData()->Write("run_data", TObject::kOverwrite); 

    fTree->ResetBranchAddresses();
    delete fTree;
    fTree = NULL;

    fFile->Close();

    delete fFile;
    fFile = NULL;

    G4cout << "written" << G4endl;

    return;
}

///////////////////////////////////////////////////////////////////////////////
// Interfaces to output section ///////////////////////////////////////////////

// Event Data

void remollIO::SetEventData(remollEvent *ev){
    int n = ev->fPartType.size();

    fNEvPart = n;

    fEvRate   = ev->fRate*s;
    fEvEffXS  = ev->fEffXs/microbarn;
    fEvAsym   = ev->fAsym/__ASYMM_SCALE;
    fEvmAsym  = ev->fmAsym/__ASYMM_SCALE;
    fEvBeamP  = ev->fBeamMomentum.mag()/__E_UNIT;

    fEvQ2     = ev->fQ2/__E_UNIT/__E_UNIT;
    fEvW2     = ev->fW2/__E_UNIT/__E_UNIT;
    fEvThCoM  = ev->fThCoM/deg; // specify this in degrees over anything else

    int idx;
    for( idx = 0; idx < n; idx++ ){
	fEvPID->push_back(ev->fPartType[idx]->GetPDGEncoding());

	fEvPart_X->push_back(ev->fPartPos[idx].x()/__L_UNIT);
	fEvPart_Y->push_back(ev->fPartPos[idx].y()/__L_UNIT);
	fEvPart_Z->push_back(ev->fPartPos[idx].z()/__L_UNIT);

	fEvPart_Px->push_back(ev->fPartRealMom[idx].x()/__E_UNIT);
	fEvPart_Py->push_back(ev->fPartRealMom[idx].y()/__E_UNIT);
	fEvPart_Pz->push_back(ev->fPartRealMom[idx].z()/__E_UNIT);
	fEvPart_Th->push_back(ev->fPartRealMom[idx].theta());
	fEvPart_Ph->push_back(ev->fPartRealMom[idx].phi());

	fEvPart_P->push_back(ev->fPartRealMom[idx].mag()/__E_UNIT);

	fEvPart_tPx->push_back(ev->fPartMom[idx].x()/__E_UNIT);
	fEvPart_tPy->push_back(ev->fPartMom[idx].y()/__E_UNIT);
	fEvPart_tPz->push_back(ev->fPartMom[idx].z()/__E_UNIT);
    }

    /////////////////////////////////////////////////
    //  Set beam data as well

    remollBeamTarget *bt = remollBeamTarget::GetBeamTarget();

    fBmX = bt->fVer.x()/__L_UNIT;
    fBmY = bt->fVer.y()/__L_UNIT;
    fBmZ = bt->fVer.z()/__L_UNIT;
    
    fBmdX = bt->fDir.x();
    fBmdY = bt->fDir.y();
    fBmdZ = bt->fDir.z();

    return;
}

// GenericDetectorHit

void remollIO::AddGenericDetectorHit(remollGenericDetectorHit *hit){

    fGenDetHit_det->push_back(hit->fDetID);
    fGenDetHit_id->push_back(hit->fCopyID);

    fGenDetHit_trid->push_back(hit->fTrID);
    fGenDetHit_mtrid->push_back(hit->fmTrID);
    fGenDetHit_pid->push_back(hit->fPID);
    fGenDetHit_gen->push_back(hit->fGen);

    fGenDetHit_X->push_back(hit->f3X.x()/__L_UNIT);
    fGenDetHit_Y->push_back(hit->f3X.y()/__L_UNIT);
    fGenDetHit_Z->push_back(hit->f3X.z()/__L_UNIT);
    fGenDetHit_R->push_back(sqrt(hit->f3X.x()*hit->f3X.x()+hit->f3X.y()*hit->f3X.y())/__L_UNIT);
    fGenDetHit_Ph->push_back(hit->f3X.phi()/deg);

    fGenDetHit_Px->push_back(hit->f3P.x()/__E_UNIT);
    fGenDetHit_Py->push_back(hit->f3P.y()/__E_UNIT);
    fGenDetHit_Pz->push_back(hit->f3P.z()/__E_UNIT);

    fGenDetHit_Vx->push_back(hit->f3V.x()/__L_UNIT);
    fGenDetHit_Vy->push_back(hit->f3V.y()/__L_UNIT);
    fGenDetHit_Vz->push_back(hit->f3V.z()/__L_UNIT);

    fGenDetHit_P->push_back(hit->fP/__E_UNIT);
    fGenDetHit_E->push_back(hit->fE/__E_UNIT);
    fGenDetHit_M->push_back(hit->fM/__E_UNIT);

    fNGenDetHit++;

    // for collimator cut
    if( (hit->fDetID==200 && hit->f3X.perp()/__L_UNIT < 0.03) || 
      	(hit->fDetID==201 && hit->f3X.perp()/__L_UNIT < 0.05) )
      fCollCut=0;
}


// GenericDetectorSum

void remollIO::AddGenericDetectorSum(remollGenericDetectorSum *hit){
    
    fGenDetSum_edep->push_back(hit->fEdep/__E_UNIT);
    fGenDetSum_det->push_back(hit->fDetID);
    fGenDetSum_id->push_back(hit->fCopyID);

    fNGenDetSum++;
}

/*---------------------------------------------------------------------------------*/

void remollIO::GrabGDMLFiles(G4String fn){
    // Reset list
    fGDMLFileNames.clear();

    remollRunData *rundata = remollRun::GetRun()->GetData();
    rundata->ClearGDMLFiles();

    xercesc::XMLPlatformUtils::Initialize();
    SearchGDMLforFiles(fn);
    xercesc::XMLPlatformUtils::Terminate();


    // Store filename

    unsigned int idx;

    // Copy into buffers
    for( idx = 0; idx < fGDMLFileNames.size(); idx++ ){
	G4cout << "Found GDML file " << fGDMLFileNames[idx] << G4endl;
	rundata->AddGDMLFile(fGDMLFileNames[idx]);
    }

    return;
}

void remollIO::SearchGDMLforFiles(G4String fn){
    /*!  Chase down files to be included by GDML.
     *   Mainly look for file tags and perform recursively */

    struct stat thisfile;

    int ret = stat(fn.data(), &thisfile);

    if( ret != 0 ){
	G4cerr << "ERROR opening file " << fn <<  " in " << __PRETTY_FUNCTION__ << ": " << strerror(errno) << G4endl;
	exit(1);
    }

   xercesc::XercesDOMParser *xmlParser = new xercesc::XercesDOMParser();

   // Make sure file exists - otherwise freak out

   fGDMLFileNames.push_back(fn.data());

   xmlParser->parse( fn.data() );
   xercesc::DOMDocument* xmlDoc = xmlParser->getDocument();

   xercesc::DOMElement* elementRoot = xmlDoc->getDocumentElement();

   TraverseChildren( elementRoot );
   return;
}

void remollIO::TraverseChildren( xercesc::DOMElement *thisel ){

   xercesc::DOMNodeList*      children = thisel->getChildNodes();
   const XMLSize_t nodeCount = children->getLength();

   for( XMLSize_t xx = 0; xx < nodeCount; ++xx ){
       xercesc::DOMNode* currentNode = children->item(xx);
       if( currentNode->getNodeType() ){   // true is not NULL

	   if( currentNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE ){ // is element 
	       xercesc::DOMElement* currentElement
		   = dynamic_cast< xercesc::DOMElement* >( currentNode );
	       if( xercesc::XMLString::equals(currentElement->getTagName(), xercesc::XMLString::transcode("file"))){
		   SearchGDMLforFiles(G4String(xercesc::XMLString::transcode(currentElement->getAttribute(xercesc::XMLString::transcode("name")))));
	       }

	       if( currentElement->getChildNodes()->getLength() > 0 ){
		   TraverseChildren( currentElement );
	       }
	   }
       }
   }

}









