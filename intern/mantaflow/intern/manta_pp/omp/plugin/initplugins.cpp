




// DO NOT EDIT !
// This file is generated using the MantaFlow preprocessor (prep generate).




#line 1 "/Users/sebbas/Developer/Mantaflow/mantaflowDevelop/mantaflowgit/source/plugin/initplugins.cpp"
/******************************************************************************
 *
 * MantaFlow fluid solver framework
 * Copyright 2011 Tobias Pfaff, Nils Thuerey 
 *
 * This program is free software, distributed under the terms of the
 * GNU General Public License (GPL) 
 * http://www.gnu.org/licenses
 *
 * Tools to setup fields and inflows
 *
 ******************************************************************************/

#include "vectorbase.h"
#include "shapes.h"
#include "commonkernels.h"
#include "particle.h"
#include "noisefield.h"
#include "simpleimage.h"
#include "mesh.h"

#define ADD_IF_LOWER_POS(a, b) (min((a) + (b), max((a), (b))))
#define ADD_IF_LOWER_NEG(a, b) (max((a) + (b), min((a), (b))))
#define ADD_IF_LOWER(a, b) (((b) > 0) ? ADD_IF_LOWER_POS((a), (b)) : ADD_IF_LOWER_NEG((a), (b)))

#define FLOW_SMOKE 0
#define FLOW_FIRE 1
#define FLOW_SMOKE_FIRE 2

using namespace std;

namespace Manta {
	
//! Apply noise to grid


 struct KnApplyNoiseInfl : public KernelBase { KnApplyNoiseInfl(FlagGrid& flags, Grid<Real>& density, WaveletNoiseField& noise, Grid<Real>& sdf, Real scale, Real sigma) :  KernelBase(&flags,0) ,flags(flags),density(density),noise(noise),sdf(sdf),scale(scale),sigma(sigma)   { runMessage(); run(); }  inline void op(int i, int j, int k, FlagGrid& flags, Grid<Real>& density, WaveletNoiseField& noise, Grid<Real>& sdf, Real scale, Real sigma )  {
	if (!flags.isFluid(i,j,k) || sdf(i,j,k) > sigma) return;
	Real factor = clamp(1.0-0.5/sigma * (sdf(i,j,k)+sigma), 0.0, 1.0);
	
	Real target = noise.evaluate(Vec3(i,j,k)) * scale * factor;
	if (density(i,j,k) < target)
		density(i,j,k) = target;
}   inline FlagGrid& getArg0() { return flags; } typedef FlagGrid type0;inline Grid<Real>& getArg1() { return density; } typedef Grid<Real> type1;inline WaveletNoiseField& getArg2() { return noise; } typedef WaveletNoiseField type2;inline Grid<Real>& getArg3() { return sdf; } typedef Grid<Real> type3;inline Real& getArg4() { return scale; } typedef Real type4;inline Real& getArg5() { return sigma; } typedef Real type5; void runMessage() { debMsg("Executing kernel KnApplyNoiseInfl ", 3); debMsg("Kernel range" <<  " x "<<  maxX  << " y "<< maxY  << " z "<< minZ<<" - "<< maxZ  << " "   , 4); }; void run() {  const int _maxX = maxX; const int _maxY = maxY; if (maxZ > 1) { 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int k=minZ; k < maxZ; k++) for (int j=0; j < _maxY; j++) for (int i=0; i < _maxX; i++) op(i,j,k,flags,density,noise,sdf,scale,sigma);  } } else { const int k=0; 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int j=0; j < _maxY; j++) for (int i=0; i < _maxX; i++) op(i,j,k,flags,density,noise,sdf,scale,sigma);  } }  } FlagGrid& flags; Grid<Real>& density; WaveletNoiseField& noise; Grid<Real>& sdf; Real scale; Real sigma;   };
#line 37 "plugin/initplugins.cpp"



//! Init noise-modulated density inside shape

void densityInflow(FlagGrid& flags, Grid<Real>& density, WaveletNoiseField& noise, Shape* shape, Real scale=1.0, Real sigma=0) {
	Grid<Real> sdf = shape->computeLevelset();
	KnApplyNoiseInfl(flags, density, noise, sdf, scale, sigma);
} static PyObject* _W_0 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "densityInflow" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; FlagGrid& flags = *_args.getPtr<FlagGrid >("flags",0,&_lock); Grid<Real>& density = *_args.getPtr<Grid<Real> >("density",1,&_lock); WaveletNoiseField& noise = *_args.getPtr<WaveletNoiseField >("noise",2,&_lock); Shape* shape = _args.getPtr<Shape >("shape",3,&_lock); Real scale = _args.getOpt<Real >("scale",4,1.0,&_lock); Real sigma = _args.getOpt<Real >("sigma",5,0,&_lock);   _retval = getPyNone(); densityInflow(flags,density,noise,shape,scale,sigma);  _args.check(); } pbFinalizePlugin(parent,"densityInflow", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("densityInflow",e.what()); return 0; } } static const Pb::Register _RP_densityInflow ("","densityInflow",_W_0);  extern "C" { void PbRegister_densityInflow() { KEEP_UNUSED(_RP_densityInflow); } } 
//! Apply noise to real grid based on an SDF
 struct KnAddNoise : public KernelBase { KnAddNoise(FlagGrid& flags, Grid<Real>& density, WaveletNoiseField& noise, Grid<Real>* sdf, Real scale) :  KernelBase(&flags,0) ,flags(flags),density(density),noise(noise),sdf(sdf),scale(scale)   { runMessage(); run(); }  inline void op(int i, int j, int k, FlagGrid& flags, Grid<Real>& density, WaveletNoiseField& noise, Grid<Real>* sdf, Real scale )  {
	if (!flags.isFluid(i,j,k) || (sdf && (*sdf)(i,j,k) > 0.) ) return;
	density(i,j,k) += noise.evaluate(Vec3(i,j,k)) * scale;
}   inline FlagGrid& getArg0() { return flags; } typedef FlagGrid type0;inline Grid<Real>& getArg1() { return density; } typedef Grid<Real> type1;inline WaveletNoiseField& getArg2() { return noise; } typedef WaveletNoiseField type2;inline Grid<Real>* getArg3() { return sdf; } typedef Grid<Real> type3;inline Real& getArg4() { return scale; } typedef Real type4; void runMessage() { debMsg("Executing kernel KnAddNoise ", 3); debMsg("Kernel range" <<  " x "<<  maxX  << " y "<< maxY  << " z "<< minZ<<" - "<< maxZ  << " "   , 4); }; void run() {  const int _maxX = maxX; const int _maxY = maxY; if (maxZ > 1) { 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int k=minZ; k < maxZ; k++) for (int j=0; j < _maxY; j++) for (int i=0; i < _maxX; i++) op(i,j,k,flags,density,noise,sdf,scale);  } } else { const int k=0; 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int j=0; j < _maxY; j++) for (int i=0; i < _maxX; i++) op(i,j,k,flags,density,noise,sdf,scale);  } }  } FlagGrid& flags; Grid<Real>& density; WaveletNoiseField& noise; Grid<Real>* sdf; Real scale;   };
#line 53 "plugin/initplugins.cpp"


void addNoise(FlagGrid& flags, Grid<Real>& density, WaveletNoiseField& noise, Grid<Real>* sdf=NULL, Real scale=1.0 ) {
	KnAddNoise(flags, density, noise, sdf, scale );
} static PyObject* _W_1 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "addNoise" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; FlagGrid& flags = *_args.getPtr<FlagGrid >("flags",0,&_lock); Grid<Real>& density = *_args.getPtr<Grid<Real> >("density",1,&_lock); WaveletNoiseField& noise = *_args.getPtr<WaveletNoiseField >("noise",2,&_lock); Grid<Real>* sdf = _args.getPtrOpt<Grid<Real> >("sdf",3,NULL,&_lock); Real scale = _args.getOpt<Real >("scale",4,1.0 ,&_lock);   _retval = getPyNone(); addNoise(flags,density,noise,sdf,scale);  _args.check(); } pbFinalizePlugin(parent,"addNoise", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("addNoise",e.what()); return 0; } } static const Pb::Register _RP_addNoise ("","addNoise",_W_1);  extern "C" { void PbRegister_addNoise() { KEEP_UNUSED(_RP_addNoise); } } 

//! sample noise field and set pdata with its values (for convenience, scale the noise values)

template <class T>  struct knSetPdataNoise : public KernelBase { knSetPdataNoise(BasicParticleSystem& parts, ParticleDataImpl<T>& pdata, WaveletNoiseField& noise, Real scale) :  KernelBase(parts.size()) ,parts(parts),pdata(pdata),noise(noise),scale(scale)   { runMessage(); run(); }   inline void op(IndexInt idx, BasicParticleSystem& parts, ParticleDataImpl<T>& pdata, WaveletNoiseField& noise, Real scale )  {
	pdata[idx] = noise.evaluate( parts.getPos(idx) ) * scale;
}    inline BasicParticleSystem& getArg0() { return parts; } typedef BasicParticleSystem type0;inline ParticleDataImpl<T>& getArg1() { return pdata; } typedef ParticleDataImpl<T> type1;inline WaveletNoiseField& getArg2() { return noise; } typedef WaveletNoiseField type2;inline Real& getArg3() { return scale; } typedef Real type3; void runMessage() { debMsg("Executing kernel knSetPdataNoise ", 3); debMsg("Kernel range" <<  " size "<<  size  << " "   , 4); }; void run() {   const IndexInt _sz = size; 
#pragma omp parallel 
 {  
#pragma omp for  
  for (IndexInt i = 0; i < _sz; i++) op(i,parts,pdata,noise,scale);  }   } BasicParticleSystem& parts; ParticleDataImpl<T>& pdata; WaveletNoiseField& noise; Real scale;   };
#line 63 "plugin/initplugins.cpp"



template <class T>  struct knSetPdataNoiseVec : public KernelBase { knSetPdataNoiseVec(BasicParticleSystem& parts, ParticleDataImpl<T>& pdata, WaveletNoiseField& noise, Real scale) :  KernelBase(parts.size()) ,parts(parts),pdata(pdata),noise(noise),scale(scale)   { runMessage(); run(); }   inline void op(IndexInt idx, BasicParticleSystem& parts, ParticleDataImpl<T>& pdata, WaveletNoiseField& noise, Real scale )  {
	pdata[idx] = noise.evaluateVec( parts.getPos(idx) ) * scale;
}    inline BasicParticleSystem& getArg0() { return parts; } typedef BasicParticleSystem type0;inline ParticleDataImpl<T>& getArg1() { return pdata; } typedef ParticleDataImpl<T> type1;inline WaveletNoiseField& getArg2() { return noise; } typedef WaveletNoiseField type2;inline Real& getArg3() { return scale; } typedef Real type3; void runMessage() { debMsg("Executing kernel knSetPdataNoiseVec ", 3); debMsg("Kernel range" <<  " size "<<  size  << " "   , 4); }; void run() {   const IndexInt _sz = size; 
#pragma omp parallel 
 {  
#pragma omp for  
  for (IndexInt i = 0; i < _sz; i++) op(i,parts,pdata,noise,scale);  }   } BasicParticleSystem& parts; ParticleDataImpl<T>& pdata; WaveletNoiseField& noise; Real scale;   };
#line 67 "plugin/initplugins.cpp"


void setNoisePdata(BasicParticleSystem& parts, ParticleDataImpl<Real>& pd, WaveletNoiseField& noise, Real scale=1.) { knSetPdataNoise<Real>(parts, pd,noise,scale); } static PyObject* _W_2 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "setNoisePdata" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; BasicParticleSystem& parts = *_args.getPtr<BasicParticleSystem >("parts",0,&_lock); ParticleDataImpl<Real>& pd = *_args.getPtr<ParticleDataImpl<Real> >("pd",1,&_lock); WaveletNoiseField& noise = *_args.getPtr<WaveletNoiseField >("noise",2,&_lock); Real scale = _args.getOpt<Real >("scale",3,1.,&_lock);   _retval = getPyNone(); setNoisePdata(parts,pd,noise,scale);  _args.check(); } pbFinalizePlugin(parent,"setNoisePdata", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("setNoisePdata",e.what()); return 0; } } static const Pb::Register _RP_setNoisePdata ("","setNoisePdata",_W_2);  extern "C" { void PbRegister_setNoisePdata() { KEEP_UNUSED(_RP_setNoisePdata); } } 
void setNoisePdataVec3(BasicParticleSystem& parts, ParticleDataImpl<Vec3>& pd, WaveletNoiseField& noise, Real scale=1.) { knSetPdataNoiseVec<Vec3>(parts, pd,noise,scale); } static PyObject* _W_3 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "setNoisePdataVec3" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; BasicParticleSystem& parts = *_args.getPtr<BasicParticleSystem >("parts",0,&_lock); ParticleDataImpl<Vec3>& pd = *_args.getPtr<ParticleDataImpl<Vec3> >("pd",1,&_lock); WaveletNoiseField& noise = *_args.getPtr<WaveletNoiseField >("noise",2,&_lock); Real scale = _args.getOpt<Real >("scale",3,1.,&_lock);   _retval = getPyNone(); setNoisePdataVec3(parts,pd,noise,scale);  _args.check(); } pbFinalizePlugin(parent,"setNoisePdataVec3", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("setNoisePdataVec3",e.what()); return 0; } } static const Pb::Register _RP_setNoisePdataVec3 ("","setNoisePdataVec3",_W_3);  extern "C" { void PbRegister_setNoisePdataVec3() { KEEP_UNUSED(_RP_setNoisePdataVec3); } } 
void setNoisePdataInt(BasicParticleSystem& parts, ParticleDataImpl<int >& pd, WaveletNoiseField& noise, Real scale=1.) { knSetPdataNoise<int> (parts, pd,noise,scale); } static PyObject* _W_4 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "setNoisePdataInt" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; BasicParticleSystem& parts = *_args.getPtr<BasicParticleSystem >("parts",0,&_lock); ParticleDataImpl<int >& pd = *_args.getPtr<ParticleDataImpl<int > >("pd",1,&_lock); WaveletNoiseField& noise = *_args.getPtr<WaveletNoiseField >("noise",2,&_lock); Real scale = _args.getOpt<Real >("scale",3,1.,&_lock);   _retval = getPyNone(); setNoisePdataInt(parts,pd,noise,scale);  _args.check(); } pbFinalizePlugin(parent,"setNoisePdataInt", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("setNoisePdataInt",e.what()); return 0; } } static const Pb::Register _RP_setNoisePdataInt ("","setNoisePdataInt",_W_4);  extern "C" { void PbRegister_setNoisePdataInt() { KEEP_UNUSED(_RP_setNoisePdataInt); } } 

//! SDF gradient from obstacle flags
Grid<Vec3> obstacleGradient(FlagGrid& flags) {
	LevelsetGrid levelset(flags.getParent(),false);
	Grid<Vec3> gradient(flags.getParent());
	
	// rebuild obstacle levelset
	FOR_IDX(levelset) {
		levelset[idx] = flags.isObstacle(idx) ? -0.5 : 0.5;
	}
	levelset.reinitMarching(flags, 6.0, 0, true, false, FlagGrid::TypeReserved);
	
	// build levelset gradient
	GradientOp(gradient, levelset);
	
	FOR_IDX(levelset) {
		Vec3 grad = gradient[idx];
		Real s = normalize(grad);
		if (s <= 0.1 || levelset[idx] >= 0) 
			grad=Vec3(0.);        
		gradient[idx] = grad * levelset[idx];
	}
	
	return gradient;
} static PyObject* _W_5 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "obstacleGradient" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; FlagGrid& flags = *_args.getPtr<FlagGrid >("flags",0,&_lock);   _retval = toPy(obstacleGradient(flags));  _args.check(); } pbFinalizePlugin(parent,"obstacleGradient", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("obstacleGradient",e.what()); return 0; } } static const Pb::Register _RP_obstacleGradient ("","obstacleGradient",_W_5);  extern "C" { void PbRegister_obstacleGradient() { KEEP_UNUSED(_RP_obstacleGradient); } } 

LevelsetGrid obstacleLevelset(FlagGrid& flags) {
   LevelsetGrid levelset(flags.getParent(),false);
	Grid<Vec3> gradient(flags.getParent());

	// rebuild obstacle levelset
	FOR_IDX(levelset) {
		levelset[idx] = flags.isObstacle(idx) ? -0.5 : 0.5;
	}
	levelset.reinitMarching(flags, 6.0, 0, true, false, FlagGrid::TypeReserved);

	return levelset;
} static PyObject* _W_6 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "obstacleLevelset" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; FlagGrid& flags = *_args.getPtr<FlagGrid >("flags",0,&_lock);   _retval = toPy(obstacleLevelset(flags));  _args.check(); } pbFinalizePlugin(parent,"obstacleLevelset", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("obstacleLevelset",e.what()); return 0; } } static const Pb::Register _RP_obstacleLevelset ("","obstacleLevelset",_W_6);  extern "C" { void PbRegister_obstacleLevelset() { KEEP_UNUSED(_RP_obstacleLevelset); } }     


//*****************************************************************************
// blender init functions 



 struct KnApplyEmission : public KernelBase { KnApplyEmission(FlagGrid& flags, Grid<Real>& density, Grid<Real>& emission, bool isAbsolute) :  KernelBase(&flags,0) ,flags(flags),density(density),emission(emission),isAbsolute(isAbsolute)   { runMessage(); run(); }  inline void op(int i, int j, int k, FlagGrid& flags, Grid<Real>& density, Grid<Real>& emission, bool isAbsolute )  {
	if (!flags.isFluid(i,j,k) || emission(i,j,k) == 0.) return;
	if (isAbsolute)
		density(i,j,k) = emission(i,j,k);
	else
		density(i,j,k) += emission(i,j,k);
}   inline FlagGrid& getArg0() { return flags; } typedef FlagGrid type0;inline Grid<Real>& getArg1() { return density; } typedef Grid<Real> type1;inline Grid<Real>& getArg2() { return emission; } typedef Grid<Real> type2;inline bool& getArg3() { return isAbsolute; } typedef bool type3; void runMessage() { debMsg("Executing kernel KnApplyEmission ", 3); debMsg("Kernel range" <<  " x "<<  maxX  << " y "<< maxY  << " z "<< minZ<<" - "<< maxZ  << " "   , 4); }; void run() {  const int _maxX = maxX; const int _maxY = maxY; if (maxZ > 1) { 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int k=minZ; k < maxZ; k++) for (int j=0; j < _maxY; j++) for (int i=0; i < _maxX; i++) op(i,j,k,flags,density,emission,isAbsolute);  } } else { const int k=0; 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int j=0; j < _maxY; j++) for (int i=0; i < _maxX; i++) op(i,j,k,flags,density,emission,isAbsolute);  } }  } FlagGrid& flags; Grid<Real>& density; Grid<Real>& emission; bool isAbsolute;   };
#line 118 "plugin/initplugins.cpp"



//! Add emission values
//isAbsolute: whether to add emission values to existing, or replace
void applyEmission(FlagGrid& flags, Grid<Real>& density, Grid<Real>& emission, bool isAbsolute) {
	KnApplyEmission(flags, density, emission, isAbsolute);
} static PyObject* _W_7 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "applyEmission" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; FlagGrid& flags = *_args.getPtr<FlagGrid >("flags",0,&_lock); Grid<Real>& density = *_args.getPtr<Grid<Real> >("density",1,&_lock); Grid<Real>& emission = *_args.getPtr<Grid<Real> >("emission",2,&_lock); bool isAbsolute = _args.get<bool >("isAbsolute",3,&_lock);   _retval = getPyNone(); applyEmission(flags,density,emission,isAbsolute);  _args.check(); } pbFinalizePlugin(parent,"applyEmission", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("applyEmission",e.what()); return 0; } } static const Pb::Register _RP_applyEmission ("","applyEmission",_W_7);  extern "C" { void PbRegister_applyEmission() { KEEP_UNUSED(_RP_applyEmission); } } 

// blender init functions for meshes



 struct KnApplyDensity : public KernelBase { KnApplyDensity(FlagGrid& flags, Grid<Real>& density, Grid<Real>& sdf, Real value, Real sigma) :  KernelBase(&flags,0) ,flags(flags),density(density),sdf(sdf),value(value),sigma(sigma)   { runMessage(); run(); }  inline void op(int i, int j, int k, FlagGrid& flags, Grid<Real>& density, Grid<Real>& sdf, Real value, Real sigma )  {
	if (!flags.isFluid(i,j,k) || sdf(i,j,k) > sigma) return;
	density(i,j,k) = value;
}   inline FlagGrid& getArg0() { return flags; } typedef FlagGrid type0;inline Grid<Real>& getArg1() { return density; } typedef Grid<Real> type1;inline Grid<Real>& getArg2() { return sdf; } typedef Grid<Real> type2;inline Real& getArg3() { return value; } typedef Real type3;inline Real& getArg4() { return sigma; } typedef Real type4; void runMessage() { debMsg("Executing kernel KnApplyDensity ", 3); debMsg("Kernel range" <<  " x "<<  maxX  << " y "<< maxY  << " z "<< minZ<<" - "<< maxZ  << " "   , 4); }; void run() {  const int _maxX = maxX; const int _maxY = maxY; if (maxZ > 1) { 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int k=minZ; k < maxZ; k++) for (int j=0; j < _maxY; j++) for (int i=0; i < _maxX; i++) op(i,j,k,flags,density,sdf,value,sigma);  } } else { const int k=0; 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int j=0; j < _maxY; j++) for (int i=0; i < _maxX; i++) op(i,j,k,flags,density,sdf,value,sigma);  } }  } FlagGrid& flags; Grid<Real>& density; Grid<Real>& sdf; Real value; Real sigma;   };
#line 136 "plugin/initplugins.cpp"


//! Init noise-modulated density inside mesh

void densityInflowMeshNoise(FlagGrid& flags, Grid<Real>& density, WaveletNoiseField& noise, Mesh* mesh, Real scale=1.0, Real sigma=0) {
	LevelsetGrid sdf(density.getParent(), false);
	mesh->computeLevelset(sdf, 1.);
	KnApplyNoiseInfl(flags, density, noise, sdf, scale, sigma);
} static PyObject* _W_8 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "densityInflowMeshNoise" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; FlagGrid& flags = *_args.getPtr<FlagGrid >("flags",0,&_lock); Grid<Real>& density = *_args.getPtr<Grid<Real> >("density",1,&_lock); WaveletNoiseField& noise = *_args.getPtr<WaveletNoiseField >("noise",2,&_lock); Mesh* mesh = _args.getPtr<Mesh >("mesh",3,&_lock); Real scale = _args.getOpt<Real >("scale",4,1.0,&_lock); Real sigma = _args.getOpt<Real >("sigma",5,0,&_lock);   _retval = getPyNone(); densityInflowMeshNoise(flags,density,noise,mesh,scale,sigma);  _args.check(); } pbFinalizePlugin(parent,"densityInflowMeshNoise", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("densityInflowMeshNoise",e.what()); return 0; } } static const Pb::Register _RP_densityInflowMeshNoise ("","densityInflowMeshNoise",_W_8);  extern "C" { void PbRegister_densityInflowMeshNoise() { KEEP_UNUSED(_RP_densityInflowMeshNoise); } } 

//! Init constant density inside mesh

void densityInflowMesh(FlagGrid& flags, Grid<Real>& density, Mesh* mesh, Real value=1., Real cutoff = 7, Real sigma=0) {
	LevelsetGrid sdf(density.getParent(), false);
	mesh->computeLevelset(sdf, 2., cutoff);
	KnApplyDensity(flags, density, sdf, value, sigma);
} static PyObject* _W_9 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "densityInflowMesh" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; FlagGrid& flags = *_args.getPtr<FlagGrid >("flags",0,&_lock); Grid<Real>& density = *_args.getPtr<Grid<Real> >("density",1,&_lock); Mesh* mesh = _args.getPtr<Mesh >("mesh",2,&_lock); Real value = _args.getOpt<Real >("value",3,1.,&_lock); Real cutoff = _args.getOpt<Real >("cutoff",4,7,&_lock); Real sigma = _args.getOpt<Real >("sigma",5,0,&_lock);   _retval = getPyNone(); densityInflowMesh(flags,density,mesh,value,cutoff,sigma);  _args.check(); } pbFinalizePlugin(parent,"densityInflowMesh", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("densityInflowMesh",e.what()); return 0; } } static const Pb::Register _RP_densityInflowMesh ("","densityInflowMesh",_W_9);  extern "C" { void PbRegister_densityInflowMesh() { KEEP_UNUSED(_RP_densityInflowMesh); } } 



 struct KnApplyInflow : public KernelBase { KnApplyInflow(Grid<Real>& density, Grid<Real>& emission, Grid<Real>* heat, Grid<Real>* fuel, Grid<Real>* react, Grid<Real>* red, Grid<Real>* green, Grid<Real>* blue, bool absoluteFlow, int flowType, Real flowDensity, Real flowFuel, Real flowTemp, Vec3 flowColor) :  KernelBase(&density,0) ,density(density),emission(emission),heat(heat),fuel(fuel),react(react),red(red),green(green),blue(blue),absoluteFlow(absoluteFlow),flowType(flowType),flowDensity(flowDensity),flowFuel(flowFuel),flowTemp(flowTemp),flowColor(flowColor)   { runMessage(); run(); }  inline void op(int i, int j, int k, Grid<Real>& density, Grid<Real>& emission, Grid<Real>* heat, Grid<Real>* fuel, Grid<Real>* react, Grid<Real>* red, Grid<Real>* green, Grid<Real>* blue, bool absoluteFlow, int flowType, Real flowDensity, Real flowFuel, Real flowTemp, Vec3 flowColor )  {
	// add smoke inflow
	Real dens_old = density(i,j,k);
	Real dens_flow = (flowType == FLOW_FIRE) ? 0.0f : emission(i,j,k) * flowDensity;
	Real fuel_flow = emission(i,j,k) * flowFuel;
	// add heat
	if (heat && emission(i,j,k) > 0.0f) {
		(*heat)(i,j,k) = ADD_IF_LOWER( (*heat)(i,j,k), flowTemp );
	}
	// absolute
	if (absoluteFlow) {
		if (flowType != FLOW_FIRE) {
			if (dens_flow > density(i,j,k))
				density(i,j,k) = dens_flow;
				}
		if (flowType != FLOW_SMOKE && fuel && fuel_flow) {
			if (fuel_flow > (*fuel)(i,j,k))
				(*fuel)(i,j,k) = fuel_flow;
				}
	}
	// additive
	else {
		if (flowType != FLOW_FIRE) {
			density(i,j,k) += dens_flow;
			clamp(density(i,j,k), (Real)0.0f, (Real)1.0f);
		}
		if (flowType != FLOW_SMOKE && fuel && flowFuel) {
			(*fuel)(i,j,k) += fuel_flow;
			clamp((*fuel)(i,j,k), (Real)0.0f, (Real)10.0f);
		}
	}
	
	// set color
	if (red && dens_flow) {
		float total_dens = density(i,j,k) / (dens_old + dens_flow);
		(*red)(i,j,k)   = ((*red)(i,j,k)   + flowColor.x * dens_flow) * total_dens;
		(*green)(i,j,k) = ((*green)(i,j,k) + flowColor.y * dens_flow) * total_dens;
		(*blue)(i,j,k)  = ((*blue)(i,j,k)  + flowColor.z * dens_flow) * total_dens;
	}
	
	// set fire reaction coordinate
	if (fuel && (*fuel)(i,j,k) > VECTOR_EPSILON) {
		/* instead of using 1.0 for all new fuel add slight falloff
		 * to reduce flow blockiness */
		Real value = 1.0f - pow(1.0f - emission(i,j,k), 2 );
		
		if (value > (*react)(i,j,k)) {
			Real f = fuel_flow / (*fuel)(i,j,k);
			(*react)(i,j,k) = value * f + (1.0f - f) * (*react)(i,j,k);
			clamp((*react)(i,j,k), (Real)0.0f, value);
		}
	}
}   inline Grid<Real>& getArg0() { return density; } typedef Grid<Real> type0;inline Grid<Real>& getArg1() { return emission; } typedef Grid<Real> type1;inline Grid<Real>* getArg2() { return heat; } typedef Grid<Real> type2;inline Grid<Real>* getArg3() { return fuel; } typedef Grid<Real> type3;inline Grid<Real>* getArg4() { return react; } typedef Grid<Real> type4;inline Grid<Real>* getArg5() { return red; } typedef Grid<Real> type5;inline Grid<Real>* getArg6() { return green; } typedef Grid<Real> type6;inline Grid<Real>* getArg7() { return blue; } typedef Grid<Real> type7;inline bool& getArg8() { return absoluteFlow; } typedef bool type8;inline int& getArg9() { return flowType; } typedef int type9;inline Real& getArg10() { return flowDensity; } typedef Real type10;inline Real& getArg11() { return flowFuel; } typedef Real type11;inline Real& getArg12() { return flowTemp; } typedef Real type12;inline Vec3& getArg13() { return flowColor; } typedef Vec3 type13; void runMessage() { debMsg("Executing kernel KnApplyInflow ", 3); debMsg("Kernel range" <<  " x "<<  maxX  << " y "<< maxY  << " z "<< minZ<<" - "<< maxZ  << " "   , 4); }; void run() {  const int _maxX = maxX; const int _maxY = maxY; if (maxZ > 1) { 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int k=minZ; k < maxZ; k++) for (int j=0; j < _maxY; j++) for (int i=0; i < _maxX; i++) op(i,j,k,density,emission,heat,fuel,react,red,green,blue,absoluteFlow,flowType,flowDensity,flowFuel,flowTemp,flowColor);  } } else { const int k=0; 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int j=0; j < _maxY; j++) for (int i=0; i < _maxX; i++) op(i,j,k,density,emission,heat,fuel,react,red,green,blue,absoluteFlow,flowType,flowDensity,flowFuel,flowTemp,flowColor);  } }  } Grid<Real>& density; Grid<Real>& emission; Grid<Real>* heat; Grid<Real>* fuel; Grid<Real>* react; Grid<Real>* red; Grid<Real>* green; Grid<Real>* blue; bool absoluteFlow; int flowType; Real flowDensity; Real flowFuel; Real flowTemp; Vec3 flowColor;   };
#line 158 "plugin/initplugins.cpp"


	

void applyInflow(Grid<Real>& density, Grid<Real>& emission, Grid<Real>* heat = NULL, Grid<Real>* fuel = NULL, Grid<Real>* react = NULL, Grid<Real>* red = NULL, Grid<Real>* green = NULL, Grid<Real>* blue = NULL, bool absoluteFlow = true, int flowType = FLOW_SMOKE_FIRE, Real flowDensity = 1.0f, Real flowFuel = 1.0f, Real flowTemp = 1.0f, Vec3 flowColor = Vec3(0.7f, 0.7f, 0.7f)) {
	KnApplyInflow(density, emission, heat, fuel, react, red, green, blue, absoluteFlow, flowType, flowDensity, flowFuel, flowTemp, flowColor);
} static PyObject* _W_10 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "applyInflow" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; Grid<Real>& density = *_args.getPtr<Grid<Real> >("density",0,&_lock); Grid<Real>& emission = *_args.getPtr<Grid<Real> >("emission",1,&_lock); Grid<Real>* heat = _args.getPtrOpt<Grid<Real> >("heat",2,NULL,&_lock); Grid<Real>* fuel = _args.getPtrOpt<Grid<Real> >("fuel",3,NULL,&_lock); Grid<Real>* react = _args.getPtrOpt<Grid<Real> >("react",4,NULL,&_lock); Grid<Real>* red = _args.getPtrOpt<Grid<Real> >("red",5,NULL,&_lock); Grid<Real>* green = _args.getPtrOpt<Grid<Real> >("green",6,NULL,&_lock); Grid<Real>* blue = _args.getPtrOpt<Grid<Real> >("blue",7,NULL,&_lock); bool absoluteFlow = _args.getOpt<bool >("absoluteFlow",8,true,&_lock); int flowType = _args.getOpt<int >("flowType",9,FLOW_SMOKE_FIRE,&_lock); Real flowDensity = _args.getOpt<Real >("flowDensity",10,1.0f,&_lock); Real flowFuel = _args.getOpt<Real >("flowFuel",11,1.0f,&_lock); Real flowTemp = _args.getOpt<Real >("flowTemp",12,1.0f,&_lock); Vec3 flowColor = _args.getOpt<Vec3 >("flowColor",13,Vec3(0.7f, 0.7f, 0.7f),&_lock);   _retval = getPyNone(); applyInflow(density,emission,heat,fuel,react,red,green,blue,absoluteFlow,flowType,flowDensity,flowFuel,flowTemp,flowColor);  _args.check(); } pbFinalizePlugin(parent,"applyInflow", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("applyInflow",e.what()); return 0; } } static const Pb::Register _RP_applyInflow ("","applyInflow",_W_10);  extern "C" { void PbRegister_applyInflow() { KEEP_UNUSED(_RP_applyInflow); } } 

//*****************************************************************************

//! check for symmetry , optionally enfore by copying

void checkSymmetry( Grid<Real>& a, Grid<Real>* err=NULL, bool symmetrize=false, int axis=0, int bound=0) {
	const int c  = axis; 
	const int s = a.getSize()[c];
	FOR_IJK(a) { 
		Vec3i idx(i,j,k), mdx(i,j,k);
		mdx[c] = s-1-idx[c];
		if( bound>0 && ((!a.isInBounds(idx,bound)) || (!a.isInBounds(mdx,bound))) ) continue;

		if(err) (*err)(idx) = fabs( (double)(a(idx) - a(mdx) ) ); 
		if(symmetrize && (idx[c]<s/2)) {
			a(idx) = a(mdx);
		}
	}
} static PyObject* _W_11 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "checkSymmetry" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; Grid<Real>& a = *_args.getPtr<Grid<Real> >("a",0,&_lock); Grid<Real>* err = _args.getPtrOpt<Grid<Real> >("err",1,NULL,&_lock); bool symmetrize = _args.getOpt<bool >("symmetrize",2,false,&_lock); int axis = _args.getOpt<int >("axis",3,0,&_lock); int bound = _args.getOpt<int >("bound",4,0,&_lock);   _retval = getPyNone(); checkSymmetry(a,err,symmetrize,axis,bound);  _args.check(); } pbFinalizePlugin(parent,"checkSymmetry", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("checkSymmetry",e.what()); return 0; } } static const Pb::Register _RP_checkSymmetry ("","checkSymmetry",_W_11);  extern "C" { void PbRegister_checkSymmetry() { KEEP_UNUSED(_RP_checkSymmetry); } } 
//! check for symmetry , mac grid version


void checkSymmetryVec3( Grid<Vec3>& a, Grid<Real>* err=NULL, bool symmetrize=false , int axis=0, int bound=0, int disable=0) {
	if(err) err->setConst(0.);

	// each dimension is measured separately for flexibility (could be combined)
	const int c  = axis;
	const int o1 = (c+1)%3;
	const int o2 = (c+2)%3;

	// x
	if(! (disable&1) ) {
		const int s = a.getSize()[c]+1; 
		FOR_IJK(a) { 
			Vec3i idx(i,j,k), mdx(i,j,k);
			mdx[c] = s-1-idx[c]; 
			if(mdx[c] >= a.getSize()[c]) continue; 
			if( bound>0 && ((!a.isInBounds(idx,bound)) || (!a.isInBounds(mdx,bound))) ) continue;

			// special case: center "line" of values , should be zero!
			if(mdx[c] == idx[c] ) {
				if(err) (*err)(idx) += fabs( (double)( a(idx)[c] ) ); 
				if(symmetrize) a(idx)[c] = 0.;
				continue; 
			}

			// note - the a(mdx) component needs to be inverted here!
			if(err) (*err)(idx) += fabs( (double)( a(idx)[c]- (a(mdx)[c]*-1.) ) ); 
			if(symmetrize && (idx[c]<s/2)) {
				a(idx)[c] = a(mdx)[c] * -1.;
			}
		}
	}

	// y
	if(! (disable&2) ) {
		const int s = a.getSize()[c];
		FOR_IJK(a) { 
			Vec3i idx(i,j,k), mdx(i,j,k);
			mdx[c] = s-1-idx[c]; 
			if( bound>0 && ((!a.isInBounds(idx,bound)) || (!a.isInBounds(mdx,bound))) ) continue;

			if(err) (*err)(idx) += fabs( (double)( a(idx)[o1]-a(mdx)[o1] ) ); 
			if(symmetrize && (idx[c]<s/2)) {
				a(idx)[o1] = a(mdx)[o1];
			}
		}
	} 

	// z
	if(! (disable&4) ) {
		const int s = a.getSize()[c];
		FOR_IJK(a) { 
			Vec3i idx(i,j,k), mdx(i,j,k);
			mdx[c] = s-1-idx[c]; 
			if( bound>0 && ((!a.isInBounds(idx,bound)) || (!a.isInBounds(mdx,bound))) ) continue;

			if(err) (*err)(idx) += fabs( (double)( a(idx)[o2]-a(mdx)[o2] ) ); 
			if(symmetrize && (idx[c]<s/2)) {
				a(idx)[o2] = a(mdx)[o2];
			}
		}
	} 

} static PyObject* _W_12 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "checkSymmetryVec3" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; Grid<Vec3>& a = *_args.getPtr<Grid<Vec3> >("a",0,&_lock); Grid<Real>* err = _args.getPtrOpt<Grid<Real> >("err",1,NULL,&_lock); bool symmetrize = _args.getOpt<bool >("symmetrize",2,false ,&_lock); int axis = _args.getOpt<int >("axis",3,0,&_lock); int bound = _args.getOpt<int >("bound",4,0,&_lock); int disable = _args.getOpt<int >("disable",5,0,&_lock);   _retval = getPyNone(); checkSymmetryVec3(a,err,symmetrize,axis,bound,disable);  _args.check(); } pbFinalizePlugin(parent,"checkSymmetryVec3", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("checkSymmetryVec3",e.what()); return 0; } } static const Pb::Register _RP_checkSymmetryVec3 ("","checkSymmetryVec3",_W_12);  extern "C" { void PbRegister_checkSymmetryVec3() { KEEP_UNUSED(_RP_checkSymmetryVec3); } } 


// from simpleimage.cpp
void projectImg( SimpleImage& img, Grid<Real>& val, int shadeMode=0, Real scale=1.);

//! output shaded (all 3 axes at once for 3D)
//! shading modes: 0 smoke, 1 surfaces

void projectPpmFull( Grid<Real>& val, string name, int shadeMode=0, Real scale=1.) {
	SimpleImage img;
	projectImg( img, val, shadeMode, scale );
	img.writePpm( name );
} static PyObject* _W_13 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "projectPpmFull" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; Grid<Real>& val = *_args.getPtr<Grid<Real> >("val",0,&_lock); string name = _args.get<string >("name",1,&_lock); int shadeMode = _args.getOpt<int >("shadeMode",2,0,&_lock); Real scale = _args.getOpt<Real >("scale",3,1.,&_lock);   _retval = getPyNone(); projectPpmFull(val,name,shadeMode,scale);  _args.check(); } pbFinalizePlugin(parent,"projectPpmFull", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("projectPpmFull",e.what()); return 0; } } static const Pb::Register _RP_projectPpmFull ("","projectPpmFull",_W_13);  extern "C" { void PbRegister_projectPpmFull() { KEEP_UNUSED(_RP_projectPpmFull); } } 

// helper functions for pdata operator tests

//! init some test particles at the origin

void addTestParts( BasicParticleSystem& parts, int num) {
	for(int i=0; i<num; ++i)
		parts.addBuffered( Vec3(0,0,0) );

	parts.doCompress();
	parts.insertBufferedParticles();
} static PyObject* _W_14 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "addTestParts" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; BasicParticleSystem& parts = *_args.getPtr<BasicParticleSystem >("parts",0,&_lock); int num = _args.get<int >("num",1,&_lock);   _retval = getPyNone(); addTestParts(parts,num);  _args.check(); } pbFinalizePlugin(parent,"addTestParts", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("addTestParts",e.what()); return 0; } } static const Pb::Register _RP_addTestParts ("","addTestParts",_W_14);  extern "C" { void PbRegister_addTestParts() { KEEP_UNUSED(_RP_addTestParts); } } 

// calculate the difference between two pdata fields (note - slow!, not parallelized)

Real pdataMaxDiff( ParticleDataBase* a, ParticleDataBase* b ) {    
	double maxVal = 0.;
	//debMsg(" PD "<< a->getType()<<"  as"<<a->getSizeSlow()<<"  bs"<<b->getSizeSlow() , 1);
	assertMsg(a->getType()     == b->getType()    , "pdataMaxDiff problem - different pdata types!");
	assertMsg(a->getSizeSlow() == b->getSizeSlow(), "pdataMaxDiff problem - different pdata sizes!");
	
	if (a->getType() & ParticleDataBase::TypeReal) 
	{
		ParticleDataImpl<Real>& av = *dynamic_cast<ParticleDataImpl<Real>*>(a);
		ParticleDataImpl<Real>& bv = *dynamic_cast<ParticleDataImpl<Real>*>(b);
		FOR_PARTS(av) {
			maxVal = std::max(maxVal, (double)fabs( av[idx]-bv[idx] ));
		}
	} else if (a->getType() & ParticleDataBase::TypeInt) 
	{
		ParticleDataImpl<int>& av = *dynamic_cast<ParticleDataImpl<int>*>(a);
		ParticleDataImpl<int>& bv = *dynamic_cast<ParticleDataImpl<int>*>(b);
		FOR_PARTS(av) {
			maxVal = std::max(maxVal, (double)fabs( (double)av[idx]-bv[idx] ));
		}
	} else if (a->getType() & ParticleDataBase::TypeVec3) {
		ParticleDataImpl<Vec3>& av = *dynamic_cast<ParticleDataImpl<Vec3>*>(a);
		ParticleDataImpl<Vec3>& bv = *dynamic_cast<ParticleDataImpl<Vec3>*>(b);
		FOR_PARTS(av) {
			double d = 0.;
			for(int c=0; c<3; ++c) { 
				d += fabs( (double)av[idx][c] - (double)bv[idx][c] );
			}
			maxVal = std::max(maxVal, d );
		}
	} else {
		errMsg("pdataMaxDiff: Grid Type is not supported (only Real, Vec3, int)");    
	}

	return maxVal;
} static PyObject* _W_15 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "pdataMaxDiff" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; ParticleDataBase* a = _args.getPtr<ParticleDataBase >("a",0,&_lock); ParticleDataBase* b = _args.getPtr<ParticleDataBase >("b",1,&_lock);   _retval = toPy(pdataMaxDiff(a,b));  _args.check(); } pbFinalizePlugin(parent,"pdataMaxDiff", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("pdataMaxDiff",e.what()); return 0; } } static const Pb::Register _RP_pdataMaxDiff ("","pdataMaxDiff",_W_15);  extern "C" { void PbRegister_pdataMaxDiff() { KEEP_UNUSED(_RP_pdataMaxDiff); } } 

//*****************************************************************************
// helper functions for volume fractions (which are needed for second order obstacle boundaries)


 struct kninitVortexVelocity : public KernelBase { kninitVortexVelocity(Grid<Real> &phiObs, MACGrid& vel, const Vec3 &center, const Real &radius) :  KernelBase(&phiObs,0) ,phiObs(phiObs),vel(vel),center(center),radius(radius)   { runMessage(); run(); }  inline void op(int i, int j, int k, Grid<Real> &phiObs, MACGrid& vel, const Vec3 &center, const Real &radius )  {
	
	if(phiObs(i,j,k) >= -1.) {

		Real dx = i - center.x; if(dx>=0) dx -= .5; else dx += .5;
		Real dy = j - center.y;
		Real r = std::sqrt(dx*dx+dy*dy);
		Real alpha = atan2(dy,dx);

		vel(i,j,k).x = -std::sin(alpha)*(r/radius);

		dx = i - center.x;
		dy = j - center.y; if(dy>=0) dy -= .5; else dy += .5;
		r = std::sqrt(dx*dx+dy*dy);
		alpha = atan2(dy,dx);

		vel(i,j,k).y = std::cos(alpha)*(r/radius);

	}

}   inline Grid<Real> & getArg0() { return phiObs; } typedef Grid<Real>  type0;inline MACGrid& getArg1() { return vel; } typedef MACGrid type1;inline const Vec3& getArg2() { return center; } typedef Vec3 type2;inline const Real& getArg3() { return radius; } typedef Real type3; void runMessage() { debMsg("Executing kernel kninitVortexVelocity ", 3); debMsg("Kernel range" <<  " x "<<  maxX  << " y "<< maxY  << " z "<< minZ<<" - "<< maxZ  << " "   , 4); }; void run() {  const int _maxX = maxX; const int _maxY = maxY; if (maxZ > 1) { 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int k=minZ; k < maxZ; k++) for (int j=0; j < _maxY; j++) for (int i=0; i < _maxX; i++) op(i,j,k,phiObs,vel,center,radius);  } } else { const int k=0; 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int j=0; j < _maxY; j++) for (int i=0; i < _maxX; i++) op(i,j,k,phiObs,vel,center,radius);  } }  } Grid<Real> & phiObs; MACGrid& vel; const Vec3& center; const Real& radius;   };
#line 370 "plugin/initplugins.cpp"



void initVortexVelocity(Grid<Real> &phiObs, MACGrid& vel, const Vec3 &center, const Real &radius) {
	kninitVortexVelocity(phiObs,  vel, center, radius);
} static PyObject* _W_16 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "initVortexVelocity" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; Grid<Real> & phiObs = *_args.getPtr<Grid<Real>  >("phiObs",0,&_lock); MACGrid& vel = *_args.getPtr<MACGrid >("vel",1,&_lock); const Vec3& center = _args.get<Vec3 >("center",2,&_lock); const Real& radius = _args.get<Real >("radius",3,&_lock);   _retval = getPyNone(); initVortexVelocity(phiObs,vel,center,radius);  _args.check(); } pbFinalizePlugin(parent,"initVortexVelocity", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("initVortexVelocity",e.what()); return 0; } } static const Pb::Register _RP_initVortexVelocity ("","initVortexVelocity",_W_16);  extern "C" { void PbRegister_initVortexVelocity() { KEEP_UNUSED(_RP_initVortexVelocity); } } 

inline static Real calcFraction(Real phi1, Real phi2)
{
	if(phi1>0. && phi2>0.) return 1.;
	if(phi1<0. && phi2<0.) return 0.;

	// make sure phi1 < phi2
	if (phi2<phi1) { Real t = phi1; phi1= phi2; phi2 = t; }
	Real denom = phi1-phi2;
	if (denom > -1e-04) return 0.5; 

	Real frac = 1. - phi1/denom;
	if(frac<0.01) frac = 0.; // skip , dont mark as fluid
	return std::max(Real(0), std::min(Real(1), frac ));
}


 struct KnUpdateFractions : public KernelBase { KnUpdateFractions(FlagGrid& flags, Grid<Real>& phiObs, MACGrid& fractions, const int &boundaryWidth) :  KernelBase(&flags,1) ,flags(flags),phiObs(phiObs),fractions(fractions),boundaryWidth(boundaryWidth)   { runMessage(); run(); }  inline void op(int i, int j, int k, FlagGrid& flags, Grid<Real>& phiObs, MACGrid& fractions, const int &boundaryWidth )  {

	// walls at domain bounds and inner objects
	fractions(i,j,k).x = calcFraction( phiObs(i,j,k) , phiObs(i-1,j,k));
	fractions(i,j,k).y = calcFraction( phiObs(i,j,k) , phiObs(i,j-1,k));
    if(phiObs.is3D()) {
	fractions(i,j,k).z = calcFraction( phiObs(i,j,k) , phiObs(i,j,k-1));
	}

	// remaining BCs at the domain boundaries 
	const int w = boundaryWidth;
	// only set if not in obstacle
 	if(phiObs(i,j,k)<0.) return;

	// x-direction boundaries
	if(i <= w+1) {                     //min x
		if( (flags.isInflow(i-1,j,k)) ||
			(flags.isOutflow(i-1,j,k)) ||
			(flags.isOpen(i-1,j,k)) ) {
				fractions(i,j,k).x = fractions(i,j,k).y = 1.; if(flags.is3D()) fractions(i,j,k).z = 1.;
		}
	}
	if(i >= flags.getSizeX()-w-2) {    //max x
		if(	(flags.isInflow(i+1,j,k)) ||
			(flags.isOutflow(i+1,j,k)) ||
			(flags.isOpen(i+1,j,k)) ) {
			fractions(i+1,j,k).x = fractions(i+1,j,k).y = 1.; if(flags.is3D()) fractions(i+1,j,k).z = 1.;
		}
	}
	// y-direction boundaries
 	if(j <= w+1) {                     //min y
		if(	(flags.isInflow(i,j-1,k)) ||
			(flags.isOutflow(i,j-1,k)) ||
			(flags.isOpen(i,j-1,k)) ) {
			fractions(i,j,k).x = fractions(i,j,k).y = 1.; if(flags.is3D()) fractions(i,j,k).z = 1.;
		}
 	}
 	if(j >= flags.getSizeY()-w-2) {      //max y
		if(	(flags.isInflow(i,j+1,k)) ||
			(flags.isOutflow(i,j+1,k)) ||
			(flags.isOpen(i,j+1,k)) ) {
			fractions(i,j+1,k).x = fractions(i,j+1,k).y = 1.; if(flags.is3D()) fractions(i,j+1,k).z = 1.;
		}
 	}
	// z-direction boundaries
	if(flags.is3D()) {
	if(k <= w+1) {                 //min z
		if(	(flags.isInflow(i,j,k-1)) ||
			(flags.isOutflow(i,j,k-1)) ||
			(flags.isOpen(i,j,k-1)) ) {
			fractions(i,j,k).x = fractions(i,j,k).y = 1.; if(flags.is3D()) fractions(i,j,k).z = 1.;
		}
	}
	if(j >= flags.getSizeZ()-w-2) { //max z
		if(	(flags.isInflow(i,j,k+1)) ||
			(flags.isOutflow(i,j,k+1)) ||
			(flags.isOpen(i,j,k+1)) ) {
			fractions(i,j,k+1).x = fractions(i,j,k+1).y = 1.; if(flags.is3D()) fractions(i,j,k+1).z = 1.;
		}
	}
	}

}   inline FlagGrid& getArg0() { return flags; } typedef FlagGrid type0;inline Grid<Real>& getArg1() { return phiObs; } typedef Grid<Real> type1;inline MACGrid& getArg2() { return fractions; } typedef MACGrid type2;inline const int& getArg3() { return boundaryWidth; } typedef int type3; void runMessage() { debMsg("Executing kernel KnUpdateFractions ", 3); debMsg("Kernel range" <<  " x "<<  maxX  << " y "<< maxY  << " z "<< minZ<<" - "<< maxZ  << " "   , 4); }; void run() {  const int _maxX = maxX; const int _maxY = maxY; if (maxZ > 1) { 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int k=minZ; k < maxZ; k++) for (int j=1; j < _maxY; j++) for (int i=1; i < _maxX; i++) op(i,j,k,flags,phiObs,fractions,boundaryWidth);  } } else { const int k=0; 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int j=1; j < _maxY; j++) for (int i=1; i < _maxX; i++) op(i,j,k,flags,phiObs,fractions,boundaryWidth);  } }  } FlagGrid& flags; Grid<Real>& phiObs; MACGrid& fractions; const int& boundaryWidth;   };
#line 412 "plugin/initplugins.cpp"



void updateFractions(FlagGrid& flags, Grid<Real>& phiObs, MACGrid& fractions, const int &boundaryWidth=0) {
	fractions.setConst( Vec3(0.) );
	KnUpdateFractions(flags, phiObs, fractions, boundaryWidth);
} static PyObject* _W_17 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "updateFractions" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; FlagGrid& flags = *_args.getPtr<FlagGrid >("flags",0,&_lock); Grid<Real>& phiObs = *_args.getPtr<Grid<Real> >("phiObs",1,&_lock); MACGrid& fractions = *_args.getPtr<MACGrid >("fractions",2,&_lock); const int& boundaryWidth = _args.getOpt<int >("boundaryWidth",3,0,&_lock);   _retval = getPyNone(); updateFractions(flags,phiObs,fractions,boundaryWidth);  _args.check(); } pbFinalizePlugin(parent,"updateFractions", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("updateFractions",e.what()); return 0; } } static const Pb::Register _RP_updateFractions ("","updateFractions",_W_17);  extern "C" { void PbRegister_updateFractions() { KEEP_UNUSED(_RP_updateFractions); } } 


 struct KnUpdateFlags : public KernelBase { KnUpdateFlags(FlagGrid& flags, Grid<Real>& phiObs, MACGrid* fractions, Grid<Real>* phiOut) :  KernelBase(&flags,1) ,flags(flags),phiObs(phiObs),fractions(fractions),phiOut(phiOut)   { runMessage(); run(); }  inline void op(int i, int j, int k, FlagGrid& flags, Grid<Real>& phiObs, MACGrid* fractions, Grid<Real>* phiOut )  {

	Real test = 0.;
	if (fractions) {
		test += fractions->get(i  ,j,k).x;
		test += fractions->get(i+1,j,k).x;
		test += fractions->get(i,j  ,k).y;
		test += fractions->get(i,j+1,k).y;
		if (flags.is3D()) {
			test += fractions->get(i,j,k  ).z;
			test += fractions->get(i,j,k+1).z; }
	}
	if (test==0. && phiObs(i,j,k) < 0.) flags(i,j,k) = FlagGrid::TypeObstacle;
	else if (phiOut && (*phiOut)(i,j,k) < 0.) flags(i,j,k) = (FlagGrid::TypeEmpty | FlagGrid::TypeOutflow);
	else flags(i,j,k) = FlagGrid::TypeEmpty;
}   inline FlagGrid& getArg0() { return flags; } typedef FlagGrid type0;inline Grid<Real>& getArg1() { return phiObs; } typedef Grid<Real> type1;inline MACGrid* getArg2() { return fractions; } typedef MACGrid type2;inline Grid<Real>* getArg3() { return phiOut; } typedef Grid<Real> type3; void runMessage() { debMsg("Executing kernel KnUpdateFlags ", 3); debMsg("Kernel range" <<  " x "<<  maxX  << " y "<< maxY  << " z "<< minZ<<" - "<< maxZ  << " "   , 4); }; void run() {  const int _maxX = maxX; const int _maxY = maxY; if (maxZ > 1) { 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int k=minZ; k < maxZ; k++) for (int j=1; j < _maxY; j++) for (int i=1; i < _maxX; i++) op(i,j,k,flags,phiObs,fractions,phiOut);  } } else { const int k=0; 
#pragma omp parallel 
 {  
#pragma omp for  
  for (int j=1; j < _maxY; j++) for (int i=1; i < _maxX; i++) op(i,j,k,flags,phiObs,fractions,phiOut);  } }  } FlagGrid& flags; Grid<Real>& phiObs; MACGrid* fractions; Grid<Real>* phiOut;   };
#line 482 "plugin/initplugins.cpp"



void updateFlags(FlagGrid& flags, Grid<Real>& phiObs, MACGrid* fractions=NULL, Grid<Real>* phiOut=NULL) {
	KnUpdateFlags(flags, phiObs, fractions, phiOut);
} static PyObject* _W_18 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "updateFlags" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; FlagGrid& flags = *_args.getPtr<FlagGrid >("flags",0,&_lock); Grid<Real>& phiObs = *_args.getPtr<Grid<Real> >("phiObs",1,&_lock); MACGrid* fractions = _args.getPtrOpt<MACGrid >("fractions",2,NULL,&_lock); Grid<Real>* phiOut = _args.getPtrOpt<Grid<Real> >("phiOut",3,NULL,&_lock);   _retval = getPyNone(); updateFlags(flags,phiObs,fractions,phiOut);  _args.check(); } pbFinalizePlugin(parent,"updateFlags", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("updateFlags",e.what()); return 0; } } static const Pb::Register _RP_updateFlags ("","updateFlags",_W_18);  extern "C" { void PbRegister_updateFlags() { KEEP_UNUSED(_RP_updateFlags); } } 


 struct KnClearInFlags : public KernelBase { KnClearInFlags(FlagGrid* flags, GridBase* grid, int type) :  KernelBase(flags,0) ,flags(flags),grid(grid),type(type)   { runMessage(); run(); }   inline void op(IndexInt idx, FlagGrid* flags, GridBase* grid, int type )  {
	if ((*flags).get(idx) & type) {
		if (grid->getType() & GridBase::TypeReal) {
			(*(Grid<Real>*) grid)[idx] = 0.;
		}
		else if (grid->getType() & GridBase::TypeInt) {
			(*(Grid<int>*) grid)[idx] = 0;
		}
		else if (grid->getType() & GridBase::TypeMAC || grid->getType() & GridBase::TypeVec3) {
			(*(Grid<Vec3>*) grid)[idx].x = 0;
			(*(Grid<Vec3>*) grid)[idx].y = 0;
			(*(Grid<Vec3>*) grid)[idx].z = 0;
		}
		else if (grid->getType() & GridBase::TypeLevelset) {
			(*(Grid<Real>*) grid)[idx] = 0.5;
		}
		else
			errMsg("ClearInFlags: Grid Type is not supported (only Real, Int, Vec3, MAC, Levelset)");
	}
}    inline FlagGrid* getArg0() { return flags; } typedef FlagGrid type0;inline GridBase* getArg1() { return grid; } typedef GridBase type1;inline int& getArg2() { return type; } typedef int type2; void runMessage() { debMsg("Executing kernel KnClearInFlags ", 3); debMsg("Kernel range" <<  " x "<<  maxX  << " y "<< maxY  << " z "<< minZ<<" - "<< maxZ  << " "   , 4); }; void run() {   const IndexInt _sz = size; 
#pragma omp parallel 
 {  
#pragma omp for  
  for (IndexInt i = 0; i < _sz; i++) op(i,flags,grid,type);  }   } FlagGrid* flags; GridBase* grid; int type;   };
#line 504 "plugin/initplugins.cpp"


	

void clearInFlags(FlagGrid* flags, GridBase* grid, int type = FlagGrid::TypeObstacle) {
	KnClearInFlags(flags, grid, type);
} static PyObject* _W_19 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "clearInFlags" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; FlagGrid* flags = _args.getPtr<FlagGrid >("flags",0,&_lock); GridBase* grid = _args.getPtr<GridBase >("grid",1,&_lock); int type = _args.getOpt<int >("type",2,FlagGrid::TypeObstacle,&_lock);   _retval = getPyNone(); clearInFlags(flags,grid,type);  _args.check(); } pbFinalizePlugin(parent,"clearInFlags", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("clearInFlags",e.what()); return 0; } } static const Pb::Register _RP_clearInFlags ("","clearInFlags",_W_19);  extern "C" { void PbRegister_clearInFlags() { KEEP_UNUSED(_RP_clearInFlags); } } 



 struct KnAverageGrid : public KernelBase { KnAverageGrid(Grid<Vec3>& grid, Grid<int>& num) :  KernelBase(&grid,0) ,grid(grid),num(num)   { runMessage(); run(); }   inline void op(IndexInt idx, Grid<Vec3>& grid, Grid<int>& num )  {
	if (num[idx]) {
		grid[idx].x /= num[idx];
		grid[idx].y /= num[idx];
		grid[idx].z /= num[idx];
	}
}    inline Grid<Vec3>& getArg0() { return grid; } typedef Grid<Vec3> type0;inline Grid<int>& getArg1() { return num; } typedef Grid<int> type1; void runMessage() { debMsg("Executing kernel KnAverageGrid ", 3); debMsg("Kernel range" <<  " x "<<  maxX  << " y "<< maxY  << " z "<< minZ<<" - "<< maxZ  << " "   , 4); }; void run() {   const IndexInt _sz = size; 
#pragma omp parallel 
 {  
#pragma omp for  
  for (IndexInt i = 0; i < _sz; i++) op(i,grid,num);  }   } Grid<Vec3>& grid; Grid<int>& num;   };
#line 532 "plugin/initplugins.cpp"


	

void averageGrid(Grid<Vec3>& grid, Grid<int>& num) {
	KnAverageGrid(grid, num);
} static PyObject* _W_20 (PyObject* _self, PyObject* _linargs, PyObject* _kwds) { try { PbArgs _args(_linargs, _kwds); FluidSolver *parent = _args.obtainParent(); bool noTiming = _args.getOpt<bool>("notiming", -1, 0); pbPreparePlugin(parent, "averageGrid" , !noTiming ); PyObject *_retval = 0; { ArgLocker _lock; Grid<Vec3>& grid = *_args.getPtr<Grid<Vec3> >("grid",0,&_lock); Grid<int>& num = *_args.getPtr<Grid<int> >("num",1,&_lock);   _retval = getPyNone(); averageGrid(grid,num);  _args.check(); } pbFinalizePlugin(parent,"averageGrid", !noTiming ); return _retval; } catch(std::exception& e) { pbSetError("averageGrid",e.what()); return 0; } } static const Pb::Register _RP_averageGrid ("","averageGrid",_W_20);  extern "C" { void PbRegister_averageGrid() { KEEP_UNUSED(_RP_averageGrid); } } 

} // namespace



