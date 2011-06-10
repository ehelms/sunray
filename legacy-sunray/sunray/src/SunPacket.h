#ifndef _SUNPACKET_H_
#define _SUNPACKET_H_
#include <iostream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
/*
 * BAND                Bandwidth        Range                Nominal Lambda (center of bandwidth)

1                5 nm                287.5 - 292.5                290 nm
.
.                                                                        UVB
.
7                5 nm                317.5 - 322.5                320 nm                
------------------------------------------------------------------------------------------------------------------------
8                5 nm                322.5 - 327.5                325 nm
.
.                                                                        UVA
.
23                5 nm                397.5 - 402.5                400 nm
------------------------------------------------------------------------------------------------------------------------
24                100 nm                402.5 - 502.5                452 nm                Blue
------------------------------------------------------------------------------------------------------------------------
25                100 nm             502.5 - 602.5                552 nm                Green
------------------------------------------------------------------------------------------------------------------------
26                100 nm                602.5 - 702.5                652 nm                Red
------------------------------------------------------------------------------------------------------------------------
27                100 nm                702.5 - 802.5                752 nm                NIR (near InfraRed) */
namespace GFX{
template<int N> class SunPacket;
enum BAND_TYPES {UVB,UVA,VIS_RED,VIS_GREEN,VIS_BLUE,NIR};
const double SunPacketSpectralFreqs[]={290,295,300,305,310,315,320,325,330,335,340,345,350,355,360,365,370,375,380,385,390,395,400,452.5,552.5,652.5,752.5};
const double SunPacketSpectralBands[]={5  ,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  100,  100,  100,  100};
const BAND_TYPES SunPacketSpectralBandTypes[]={UVB,UVB,UVB,UVB,UVB,UVB,UVB,UVA,UVA,UVA,UVA,UVA,UVA,UVA,UVA,UVA,UVA,UVA,UVA,UVA,UVA,UVA,UVA,VIS_BLUE,VIS_GREEN,VIS_RED,NIR};
template<int N> class SunPacket
{
	private:
		double spectralBands[N];
public:
	SunPacket(const double *freqs=NULL){
		for(int i=0;i<N;i++){
			spectralBands[i]=(freqs)?freqs[i]:0;
		}
	}
	SunPacket(const int *freqs){
		for(int i=0;i<N;i++){
			spectralBands[i]=(freqs)?freqs[i]:0;
		}
	}
	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive &ar, const unsigned int version){
        ar & BOOST_SERIALIZATION_NVP(spectralBands);
	}
	inline double* getBands(){return spectralBands;}
	inline void set(double val,int band){assert(band<N);spectralBands[band]=val;}
	inline void set(double* vals){
		for(int i=0;i<N;i++){
			spectralBands[i]=vals[i];
		}
	}
	double get(int band) const {return spectralBands[band];}
	inline void add(SunPacket<N> packet){
		for(int i=0;i<N;i++){
			spectralBands[i]+=packet.get(i);
		}
	}
	inline void subtract(SunPacket<N> packet){
		for(int i=0;i<N;i++){
			spectralBands[i]+=packet.get(i);
		}
	}
	inline double getBroadband() const {
		double broad=0;
		for(int i=0;i<N;i++){
			broad+=spectralBands[i];
		}
		return broad;
	}
	inline double* getArray(){
		return spectralBands;
	}
	inline void reset(){
		for(int i=0;i<N;i++)spectralBands[i]=0;
	}
	inline int size() const {return N;}
	inline SunPacket<N>& scale(double f){
		for(int i=0;i<N;i++)spectralBands[i]*=f;
		return *this;
	}
	inline SunPacket<N>& scale(const SunPacket<N> &p){
		for(int i=0;i<N;i++)spectralBands[i]*=p.get(i);
		return *this;
	}
	double& operator[](long index){
		return spectralBands[index];
	}

};
inline std::ostream& operator<<(std::ostream& os,const SunPacket<27> &p){
	for(int i=0;i<27;i++){
		os<<p.get(i)<<",";
	}
	return os;
}

inline bool operator<(const SunPacket<27> &p1,const SunPacket<27> &p2){
	return (p1.getBroadband()<p2.getBroadband());
}
inline bool operator<=(const SunPacket<27> &p1,const SunPacket<27> &p2){
	return (p1.getBroadband()<=p2.getBroadband());
}
inline bool operator>(const SunPacket<27> &p1,const SunPacket<27> &p2){
	return (p1.getBroadband()>p2.getBroadband());
}
inline bool operator>=(const SunPacket<27> &p1,const SunPacket<27> &p2){
	return (p1.getBroadband()>=p2.getBroadband());
}
inline bool operator==(const SunPacket<27> &p1,const SunPacket<27> &p2){
	for(int i=0;i<27;i++){
		if(p1.get(i)!=p2.get(i))return false;
	}
	return true;
}
const SunPacket<27> SpectralFreqs(SunPacketSpectralFreqs);
const SunPacket<27> SpectralBands(SunPacketSpectralBands);
}
#endif // _SUNPACKET_H_
