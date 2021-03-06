




// DO NOT EDIT !
// This file is generated using the MantaFlow preprocessor (prep link).




#include "grid4d.h"
namespace Manta {
#ifdef _C_Grid4d
 static const Pb::Register _R_12 ("Grid4d<int>","Grid4d<int>","Grid4dBase"); template<> const char* Namify<Grid4d<int> >::S = "Grid4d<int>"; 
 static const Pb::Register _R_13 ("Grid4d<int>","Grid4d",Grid4d<int>::_W_1); 
 static const Pb::Register _R_14 ("Grid4d<int>","save",Grid4d<int>::_W_2); 
 static const Pb::Register _R_15 ("Grid4d<int>","load",Grid4d<int>::_W_3); 
 static const Pb::Register _R_16 ("Grid4d<int>","clear",Grid4d<int>::_W_4); 
 static const Pb::Register _R_17 ("Grid4d<int>","copyFrom",Grid4d<int>::_W_5); 
 static const Pb::Register _R_18 ("Grid4d<int>","add",Grid4d<int>::_W_6); 
 static const Pb::Register _R_19 ("Grid4d<int>","sub",Grid4d<int>::_W_7); 
 static const Pb::Register _R_20 ("Grid4d<int>","setConst",Grid4d<int>::_W_8); 
 static const Pb::Register _R_21 ("Grid4d<int>","addConst",Grid4d<int>::_W_9); 
 static const Pb::Register _R_22 ("Grid4d<int>","addScaled",Grid4d<int>::_W_10); 
 static const Pb::Register _R_23 ("Grid4d<int>","mult",Grid4d<int>::_W_11); 
 static const Pb::Register _R_24 ("Grid4d<int>","multConst",Grid4d<int>::_W_12); 
 static const Pb::Register _R_25 ("Grid4d<int>","clamp",Grid4d<int>::_W_13); 
 static const Pb::Register _R_26 ("Grid4d<int>","getMaxAbs",Grid4d<int>::_W_14); 
 static const Pb::Register _R_27 ("Grid4d<int>","getMax",Grid4d<int>::_W_15); 
 static const Pb::Register _R_28 ("Grid4d<int>","getMin",Grid4d<int>::_W_16); 
 static const Pb::Register _R_29 ("Grid4d<int>","setBound",Grid4d<int>::_W_17); 
 static const Pb::Register _R_30 ("Grid4d<int>","setBoundNeumann",Grid4d<int>::_W_18); 
 static const Pb::Register _R_31 ("Grid4d<int>","getMaxAbsValue",Grid4d<int>::_W_19); 
 static const Pb::Register _R_32 ("Grid4d<int>","getMaxValue",Grid4d<int>::_W_20); 
 static const Pb::Register _R_33 ("Grid4d<int>","getMinValue",Grid4d<int>::_W_21); 
 static const Pb::Register _R_34 ("Grid4d<int>","printGrid",Grid4d<int>::_W_22); 
 static const Pb::Register _R_35 ("Grid4d<Real>","Grid4d<Real>","Grid4dBase"); template<> const char* Namify<Grid4d<Real> >::S = "Grid4d<Real>"; 
 static const Pb::Register _R_36 ("Grid4d<Real>","Grid4d",Grid4d<Real>::_W_1); 
 static const Pb::Register _R_37 ("Grid4d<Real>","save",Grid4d<Real>::_W_2); 
 static const Pb::Register _R_38 ("Grid4d<Real>","load",Grid4d<Real>::_W_3); 
 static const Pb::Register _R_39 ("Grid4d<Real>","clear",Grid4d<Real>::_W_4); 
 static const Pb::Register _R_40 ("Grid4d<Real>","copyFrom",Grid4d<Real>::_W_5); 
 static const Pb::Register _R_41 ("Grid4d<Real>","add",Grid4d<Real>::_W_6); 
 static const Pb::Register _R_42 ("Grid4d<Real>","sub",Grid4d<Real>::_W_7); 
 static const Pb::Register _R_43 ("Grid4d<Real>","setConst",Grid4d<Real>::_W_8); 
 static const Pb::Register _R_44 ("Grid4d<Real>","addConst",Grid4d<Real>::_W_9); 
 static const Pb::Register _R_45 ("Grid4d<Real>","addScaled",Grid4d<Real>::_W_10); 
 static const Pb::Register _R_46 ("Grid4d<Real>","mult",Grid4d<Real>::_W_11); 
 static const Pb::Register _R_47 ("Grid4d<Real>","multConst",Grid4d<Real>::_W_12); 
 static const Pb::Register _R_48 ("Grid4d<Real>","clamp",Grid4d<Real>::_W_13); 
 static const Pb::Register _R_49 ("Grid4d<Real>","getMaxAbs",Grid4d<Real>::_W_14); 
 static const Pb::Register _R_50 ("Grid4d<Real>","getMax",Grid4d<Real>::_W_15); 
 static const Pb::Register _R_51 ("Grid4d<Real>","getMin",Grid4d<Real>::_W_16); 
 static const Pb::Register _R_52 ("Grid4d<Real>","setBound",Grid4d<Real>::_W_17); 
 static const Pb::Register _R_53 ("Grid4d<Real>","setBoundNeumann",Grid4d<Real>::_W_18); 
 static const Pb::Register _R_54 ("Grid4d<Real>","getMaxAbsValue",Grid4d<Real>::_W_19); 
 static const Pb::Register _R_55 ("Grid4d<Real>","getMaxValue",Grid4d<Real>::_W_20); 
 static const Pb::Register _R_56 ("Grid4d<Real>","getMinValue",Grid4d<Real>::_W_21); 
 static const Pb::Register _R_57 ("Grid4d<Real>","printGrid",Grid4d<Real>::_W_22); 
 static const Pb::Register _R_58 ("Grid4d<Vec3>","Grid4d<Vec3>","Grid4dBase"); template<> const char* Namify<Grid4d<Vec3> >::S = "Grid4d<Vec3>"; 
 static const Pb::Register _R_59 ("Grid4d<Vec3>","Grid4d",Grid4d<Vec3>::_W_1); 
 static const Pb::Register _R_60 ("Grid4d<Vec3>","save",Grid4d<Vec3>::_W_2); 
 static const Pb::Register _R_61 ("Grid4d<Vec3>","load",Grid4d<Vec3>::_W_3); 
 static const Pb::Register _R_62 ("Grid4d<Vec3>","clear",Grid4d<Vec3>::_W_4); 
 static const Pb::Register _R_63 ("Grid4d<Vec3>","copyFrom",Grid4d<Vec3>::_W_5); 
 static const Pb::Register _R_64 ("Grid4d<Vec3>","add",Grid4d<Vec3>::_W_6); 
 static const Pb::Register _R_65 ("Grid4d<Vec3>","sub",Grid4d<Vec3>::_W_7); 
 static const Pb::Register _R_66 ("Grid4d<Vec3>","setConst",Grid4d<Vec3>::_W_8); 
 static const Pb::Register _R_67 ("Grid4d<Vec3>","addConst",Grid4d<Vec3>::_W_9); 
 static const Pb::Register _R_68 ("Grid4d<Vec3>","addScaled",Grid4d<Vec3>::_W_10); 
 static const Pb::Register _R_69 ("Grid4d<Vec3>","mult",Grid4d<Vec3>::_W_11); 
 static const Pb::Register _R_70 ("Grid4d<Vec3>","multConst",Grid4d<Vec3>::_W_12); 
 static const Pb::Register _R_71 ("Grid4d<Vec3>","clamp",Grid4d<Vec3>::_W_13); 
 static const Pb::Register _R_72 ("Grid4d<Vec3>","getMaxAbs",Grid4d<Vec3>::_W_14); 
 static const Pb::Register _R_73 ("Grid4d<Vec3>","getMax",Grid4d<Vec3>::_W_15); 
 static const Pb::Register _R_74 ("Grid4d<Vec3>","getMin",Grid4d<Vec3>::_W_16); 
 static const Pb::Register _R_75 ("Grid4d<Vec3>","setBound",Grid4d<Vec3>::_W_17); 
 static const Pb::Register _R_76 ("Grid4d<Vec3>","setBoundNeumann",Grid4d<Vec3>::_W_18); 
 static const Pb::Register _R_77 ("Grid4d<Vec3>","getMaxAbsValue",Grid4d<Vec3>::_W_19); 
 static const Pb::Register _R_78 ("Grid4d<Vec3>","getMaxValue",Grid4d<Vec3>::_W_20); 
 static const Pb::Register _R_79 ("Grid4d<Vec3>","getMinValue",Grid4d<Vec3>::_W_21); 
 static const Pb::Register _R_80 ("Grid4d<Vec3>","printGrid",Grid4d<Vec3>::_W_22); 
 static const Pb::Register _R_81 ("Grid4d<Vec4>","Grid4d<Vec4>","Grid4dBase"); template<> const char* Namify<Grid4d<Vec4> >::S = "Grid4d<Vec4>"; 
 static const Pb::Register _R_82 ("Grid4d<Vec4>","Grid4d",Grid4d<Vec4>::_W_1); 
 static const Pb::Register _R_83 ("Grid4d<Vec4>","save",Grid4d<Vec4>::_W_2); 
 static const Pb::Register _R_84 ("Grid4d<Vec4>","load",Grid4d<Vec4>::_W_3); 
 static const Pb::Register _R_85 ("Grid4d<Vec4>","clear",Grid4d<Vec4>::_W_4); 
 static const Pb::Register _R_86 ("Grid4d<Vec4>","copyFrom",Grid4d<Vec4>::_W_5); 
 static const Pb::Register _R_87 ("Grid4d<Vec4>","add",Grid4d<Vec4>::_W_6); 
 static const Pb::Register _R_88 ("Grid4d<Vec4>","sub",Grid4d<Vec4>::_W_7); 
 static const Pb::Register _R_89 ("Grid4d<Vec4>","setConst",Grid4d<Vec4>::_W_8); 
 static const Pb::Register _R_90 ("Grid4d<Vec4>","addConst",Grid4d<Vec4>::_W_9); 
 static const Pb::Register _R_91 ("Grid4d<Vec4>","addScaled",Grid4d<Vec4>::_W_10); 
 static const Pb::Register _R_92 ("Grid4d<Vec4>","mult",Grid4d<Vec4>::_W_11); 
 static const Pb::Register _R_93 ("Grid4d<Vec4>","multConst",Grid4d<Vec4>::_W_12); 
 static const Pb::Register _R_94 ("Grid4d<Vec4>","clamp",Grid4d<Vec4>::_W_13); 
 static const Pb::Register _R_95 ("Grid4d<Vec4>","getMaxAbs",Grid4d<Vec4>::_W_14); 
 static const Pb::Register _R_96 ("Grid4d<Vec4>","getMax",Grid4d<Vec4>::_W_15); 
 static const Pb::Register _R_97 ("Grid4d<Vec4>","getMin",Grid4d<Vec4>::_W_16); 
 static const Pb::Register _R_98 ("Grid4d<Vec4>","setBound",Grid4d<Vec4>::_W_17); 
 static const Pb::Register _R_99 ("Grid4d<Vec4>","setBoundNeumann",Grid4d<Vec4>::_W_18); 
 static const Pb::Register _R_100 ("Grid4d<Vec4>","getMaxAbsValue",Grid4d<Vec4>::_W_19); 
 static const Pb::Register _R_101 ("Grid4d<Vec4>","getMaxValue",Grid4d<Vec4>::_W_20); 
 static const Pb::Register _R_102 ("Grid4d<Vec4>","getMinValue",Grid4d<Vec4>::_W_21); 
 static const Pb::Register _R_103 ("Grid4d<Vec4>","printGrid",Grid4d<Vec4>::_W_22); 
#endif
#ifdef _C_Grid4dBase
 static const Pb::Register _R_104 ("Grid4dBase","Grid4dBase","PbClass"); template<> const char* Namify<Grid4dBase >::S = "Grid4dBase"; 
 static const Pb::Register _R_105 ("Grid4dBase","Grid4dBase",Grid4dBase::_W_0); 
#endif
static const Pb::Register _R_8 ("Grid4d<int>","Grid4Int","");
static const Pb::Register _R_9 ("Grid4d<Real>","Grid4Real","");
static const Pb::Register _R_10 ("Grid4d<Vec3>","Grid4Vec3","");
static const Pb::Register _R_11 ("Grid4d<Vec4>","Grid4Vec4","");
extern "C" {
void PbRegister_file_8()
{
	KEEP_UNUSED(_R_12);
	KEEP_UNUSED(_R_13);
	KEEP_UNUSED(_R_14);
	KEEP_UNUSED(_R_15);
	KEEP_UNUSED(_R_16);
	KEEP_UNUSED(_R_17);
	KEEP_UNUSED(_R_18);
	KEEP_UNUSED(_R_19);
	KEEP_UNUSED(_R_20);
	KEEP_UNUSED(_R_21);
	KEEP_UNUSED(_R_22);
	KEEP_UNUSED(_R_23);
	KEEP_UNUSED(_R_24);
	KEEP_UNUSED(_R_25);
	KEEP_UNUSED(_R_26);
	KEEP_UNUSED(_R_27);
	KEEP_UNUSED(_R_28);
	KEEP_UNUSED(_R_29);
	KEEP_UNUSED(_R_30);
	KEEP_UNUSED(_R_31);
	KEEP_UNUSED(_R_32);
	KEEP_UNUSED(_R_33);
	KEEP_UNUSED(_R_34);
	KEEP_UNUSED(_R_35);
	KEEP_UNUSED(_R_36);
	KEEP_UNUSED(_R_37);
	KEEP_UNUSED(_R_38);
	KEEP_UNUSED(_R_39);
	KEEP_UNUSED(_R_40);
	KEEP_UNUSED(_R_41);
	KEEP_UNUSED(_R_42);
	KEEP_UNUSED(_R_43);
	KEEP_UNUSED(_R_44);
	KEEP_UNUSED(_R_45);
	KEEP_UNUSED(_R_46);
	KEEP_UNUSED(_R_47);
	KEEP_UNUSED(_R_48);
	KEEP_UNUSED(_R_49);
	KEEP_UNUSED(_R_50);
	KEEP_UNUSED(_R_51);
	KEEP_UNUSED(_R_52);
	KEEP_UNUSED(_R_53);
	KEEP_UNUSED(_R_54);
	KEEP_UNUSED(_R_55);
	KEEP_UNUSED(_R_56);
	KEEP_UNUSED(_R_57);
	KEEP_UNUSED(_R_58);
	KEEP_UNUSED(_R_59);
	KEEP_UNUSED(_R_60);
	KEEP_UNUSED(_R_61);
	KEEP_UNUSED(_R_62);
	KEEP_UNUSED(_R_63);
	KEEP_UNUSED(_R_64);
	KEEP_UNUSED(_R_65);
	KEEP_UNUSED(_R_66);
	KEEP_UNUSED(_R_67);
	KEEP_UNUSED(_R_68);
	KEEP_UNUSED(_R_69);
	KEEP_UNUSED(_R_70);
	KEEP_UNUSED(_R_71);
	KEEP_UNUSED(_R_72);
	KEEP_UNUSED(_R_73);
	KEEP_UNUSED(_R_74);
	KEEP_UNUSED(_R_75);
	KEEP_UNUSED(_R_76);
	KEEP_UNUSED(_R_77);
	KEEP_UNUSED(_R_78);
	KEEP_UNUSED(_R_79);
	KEEP_UNUSED(_R_80);
	KEEP_UNUSED(_R_81);
	KEEP_UNUSED(_R_82);
	KEEP_UNUSED(_R_83);
	KEEP_UNUSED(_R_84);
	KEEP_UNUSED(_R_85);
	KEEP_UNUSED(_R_86);
	KEEP_UNUSED(_R_87);
	KEEP_UNUSED(_R_88);
	KEEP_UNUSED(_R_89);
	KEEP_UNUSED(_R_90);
	KEEP_UNUSED(_R_91);
	KEEP_UNUSED(_R_92);
	KEEP_UNUSED(_R_93);
	KEEP_UNUSED(_R_94);
	KEEP_UNUSED(_R_95);
	KEEP_UNUSED(_R_96);
	KEEP_UNUSED(_R_97);
	KEEP_UNUSED(_R_98);
	KEEP_UNUSED(_R_99);
	KEEP_UNUSED(_R_100);
	KEEP_UNUSED(_R_101);
	KEEP_UNUSED(_R_102);
	KEEP_UNUSED(_R_103);
	KEEP_UNUSED(_R_104);
	KEEP_UNUSED(_R_105);
}
}}