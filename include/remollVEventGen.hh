#ifndef __REMOLLVEVENTGEN_HH
#define __REMOLLVEVENTGEN_HH

#include "remolltypes.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "remollVertex.hh"

/*!
   Generic base class for event generators
   This provides an interface for everyone to
   derive from.

   Ultimately this returns a remollEvent which is
   what the PrimaryGeneratorAction is going to use and
   contains information that will go in the output.

   It needs to be aware of remollBeamTarget and remollRun,
   take a generically generated event assuming ideal beam
   and transform it into what is going to be simulated.
*/

class remollEvent;
class remollBeamTarget;
class remollRun;

class remollVEventGen {
    public:
	remollVEventGen();
	virtual ~remollVEventGen();

	remollEvent *GenerateEvent();

	G4String GetName(){ return fName; }

	void SetSampType( SampType_t st ) { fSampType = st; }
	void SetDoMultScatt( G4bool ms ){ fApplyMultScatt = ms; }

    private:
	const G4String fName;

	remollBeamTarget *fBeamTarg;
	remollRun        *fRun;

	SampType_t fSampType;
	G4bool     fApplyMultScatt;


	// Pure virtual function that needs to be filled out
	virtual remollEvent *SamplePhysics(remollVertex *) = 0;

	void PolishEvent(remollEvent *);
};


#endif//__REMOLLVEVENTGEN_HH
