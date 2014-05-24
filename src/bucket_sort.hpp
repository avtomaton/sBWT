#ifndef BUCKET_SORT_HPP_
#define BUCKET_SORT_HPP_
#include <fstream>
#include <map>
#include <list>
#include <random>
#include <algorithm>
#include <functional>
#include <boost/ref.hpp>
#include <utility>
#include <cstdlib>
#include <queue>
#include <tuple>
#include <ctime>
#include <unordered_map>
#include <cstring>
#include "boost/unordered_map.hpp"

//#define INTTYPE uint32_t
//#define INTMOVE 2

//#define INTTYPE uint64_t
#define INTMOVE 3

template <INTTYPE SIZE, std::string* ALPHABET>
struct Bucket
{
	INTTYPE size;
	std::vector<char> bucket_char;
	std::vector< std::vector<INTTYPE> > bucket;
	Bucket()
		:size (SIZE)
	{
		for ( size_t i=0; i<ALPHABET->length(); i++)
			bucket_char.push_back( (*ALPHABET)[i] );

		INTTYPE j=0;
		for ( INTTYPE i =0; i< 256; i++)
		{
			if ( (char)i == bucket_char[j] )
			{
				bucket.push_back( std::vector<INTTYPE>(size) );
				j++;
			}
			else
				bucket.push_back( std::vector<INTTYPE>() );
		}
				
	}
	

};



// SORT_BIG_N

//Bucket_sort2

template	<	typename SEQTYPE, 
						typename VECTORTYPE,
						typename RECORD_RANK,
						template < typename T> class COMPARE,
						template < typename T, typename Q > class SORT_SMALL_N
					>
class Sort_big_n_disable
	:public RECORD_RANK,
					COMPARE <SEQTYPE>,
					SORT_SMALL_N <SEQTYPE, VECTORTYPE>
{
private:
	SEQTYPE &seq;
	VECTORTYPE &vec;
	std::function<bool(INTTYPE,INTTYPE)> dcs_compare;
	INTTYPE limit_depth, max_depth;
	
public:
	std::function<INTTYPE(typename VECTORTYPE::value_type&)> getTableV;
	std::vector<std::pair<INTTYPE,INTTYPE>> &iD_same_rank, tmp_same_rank;;
	
	Sort_big_n_disable(SEQTYPE &sequence, VECTORTYPE &d, INTTYPE limit, std::vector<std::pair<INTTYPE,INTTYPE>> &id_same_rank)
		:	vec(d),
			seq(sequence),
			limit_depth(limit),
			RECORD_RANK(id_same_rank),
			COMPARE <SEQTYPE> (sequence, dcs_compare, limit),
			SORT_SMALL_N<SEQTYPE, VECTORTYPE>(sequence, getTableV, limit, dcs_compare),
			iD_same_rank(id_same_rank)
	{}
	
	Sort_big_n_disable(SEQTYPE &sequence, VECTORTYPE &d, INTTYPE limit, std::function<bool(INTTYPE,INTTYPE)> &compare_func)
		:	vec(d),
			seq(sequence),
			limit_depth(limit),
			RECORD_RANK(iD_same_rank),
			dcs_compare(compare_func),
			COMPARE <SEQTYPE> (sequence, compare_func, limit),
			SORT_SMALL_N<SEQTYPE, VECTORTYPE>(sequence, getTableV, limit, compare_func),
			iD_same_rank(tmp_same_rank)
	{}
	template <typename T>
	void free( T & t ) {
			T tmp;
			t.swap( tmp );
	}
	void release()
	{
		//free(bucket_data);
	}
	bool sort(INTTYPE start, INTTYPE size, INTTYPE depth)
	{
		return false;
	}

//
};
//


template	<	typename SEQTYPE, 
						typename VECTORTYPE,
						typename RECORD_RANK,
						template < typename T> class COMPARE,
						template < typename T, typename Q > class SORT_SMALL_N
//						typename BUCKET = Bucket<4*1024*1024, &ALPHABET_$ACGNT>
						//typename BUCKET = Bucket< 4 /*4*1024*1024*/, gAlphabet
					>
class Bucket_sort
	:public RECORD_RANK,
					COMPARE <SEQTYPE>,
					SORT_SMALL_N <SEQTYPE, VECTORTYPE>
{
private:
	SEQTYPE &seq;
	VECTORTYPE &vec;
	std::function<bool(INTTYPE,INTTYPE)> dcs_compare;
	INTTYPE limit_depth, max_depth;
	//BUCKET bucket_data;
	
	std::vector<char> bucket_char;
	std::vector< std::vector<INTTYPE> > bucket;
	//INTTYPE** bucket; 

public:
	std::function<INTTYPE(typename VECTORTYPE::value_type&)> getTableV;
	std::vector<std::pair<INTTYPE,INTTYPE>> &iD_same_rank, tmp_same_rank;
	
	Bucket_sort(SEQTYPE &sequence, VECTORTYPE &d, INTTYPE limit, std::vector<std::pair<INTTYPE,INTTYPE>> &id_same_rank)
		:	vec(d),
			seq(sequence),
			limit_depth(limit),
			RECORD_RANK(id_same_rank),
			COMPARE <SEQTYPE> (sequence, dcs_compare, limit),
			SORT_SMALL_N<SEQTYPE, VECTORTYPE>(sequence, getTableV, limit, dcs_compare),
			iD_same_rank(id_same_rank)
			//bucket_data()
	{
		getTableV = [](typename VECTORTYPE::value_type& a){
			return a;
		};
		
		init();
	}
	
	Bucket_sort(SEQTYPE &sequence, VECTORTYPE &d, INTTYPE limit, std::function<bool(INTTYPE,INTTYPE)> compare_func)
		:	vec(d),
			seq(sequence),
			limit_depth(limit),
			RECORD_RANK(iD_same_rank),
			dcs_compare(compare_func),
			COMPARE <SEQTYPE> (sequence, dcs_compare, limit),
			SORT_SMALL_N<SEQTYPE, VECTORTYPE>(sequence, getTableV, limit, dcs_compare),
			iD_same_rank(tmp_same_rank)
			//bucket_data()
	{
		getTableV = [](typename VECTORTYPE::value_type& a){
			return a;
		};
		init();
	}
	
	void init()
	{
		bucket_char = {'$','A','C','G','N','T'};
		bucket = 
		{ 
			{std::vector<INTTYPE>(1)}, //$
			{std::vector<INTTYPE>(4 * 1024 * 1024)}, 
			{std::vector<INTTYPE>(4 * 1024 * 1024)},
			{std::vector<INTTYPE>(4 * 1024 * 1024)},
			{std::vector<INTTYPE>(4 * 1024 * 1024)},
			{std::vector<INTTYPE>(4 * 1024 * 1024)}
		};
		/*bucket = new INTTYPE*[6];
		bucket[0] = new INTTYPE[1];
		bucket[1] = new INTTYPE[4*1024*1024];
		bucket[2] = new INTTYPE[4*1024*1024];
		bucket[3] = new INTTYPE[4*1024*1024];
		bucket[4] = new INTTYPE[4*1024*1024];
		bucket[5] = new INTTYPE[4*1024*1024];
		*/
	}
	//
	template <typename T>
	void free( T & t ) {
			T tmp;
			t.swap( tmp );
	}
	void release()
	{
		//for(auto &K : bucket)
		//{
		//	K.clear();
		//}
		std::cerr << "release bucket" << std::endl;
		free(bucket);
		//bucket.resize(0);
		std::cerr << "release bucket finish" << std::endl;
		//free(bucket_data.bucket);
	}
	
	inline bool sort(INTTYPE start, INTTYPE size, INTTYPE depth)
	{
		//if( size > bucket_data.size )
		//	return false;
		//if( size > (4 * 1024 * 1024) )
		if( size > (4 * 1024 * 1024) )
			return false;
		bucket_sort(start,start+size,depth);
		return true;
	}
	
	
	inline void bucket_sort(INTTYPE begin, INTTYPE end, INTTYPE depth)
	{
			INTTYPE n(end - begin);
			
			if (n <= 1)
			 return;
			
			if(depth == limit_depth)
			{
				this->record_rank(begin,end-1);
				this->final_sort(vec.begin()+begin, vec.begin()+end);
				return;
			}
			if(this->sort_small_n(vec.begin()+begin, vec.begin()+end, depth, n))
				return;
			
			INTTYPE vec_value;
			char seq_char;
			//std::unordered_map< char,INTTYPE > bucket_count = {{'$',0},{'A',0},{'C',0},{'G',0},{'N',0},{'T',0}};
			
			INTTYPE count_$(0), count_A(0), count_C(0), count_G(0), count_N(0), count_T(0);

			/*
			std::vector<INTTYPE> bucket_count (256, 0);		
			for(INTTYPE i(begin); i < end; ++i)
			{
				vec_value = vec[i];
				seq_char = seq[ vec_value + depth];
				bucket_data.bucket[ seq_char ][ bucket_count[seq_char]++ ] = vec_value;
			}
			INTTYPE vec_count(begin);
			std::vector<INTTYPE>::iterator bucket_iter;
			for (char c : bucket_data.bucket_char)
			{
				//std::cerr << (end-begin) <<":" << vec_count << ":" << bucket_count[c] << std::endl;
				bucket_iter = bucket_data.bucket[c].begin();
				std::swap_ranges(bucket_iter, bucket_iter + bucket_count[c], vec.begin()+vec_count );
				vec_count += bucket_count[c];
			}
			vec_count = begin;
			for (char c : bucket_data.bucket_char)
			{
				//std::cerr << "IN:" << vec_count << ":" << bucket_count[c] << ":" << c << ":" << begin << std::endl; 
				bucket_sort(vec_count, vec_count + bucket_count[c], depth+1);
				vec_count += bucket_count[c];
			}
			*/

			//for(INTTYPE i(begin); i<end; ++i)
			for(INTTYPE i(n+1); --i;)
			{
				vec_value = vec[end-i];
				//vec_value = vec[i];
				seq_char = seq[ vec_value + depth];
				//bucket[ seq_char ][ bucket_count[seq_char]++ ] = vec_value;
				if(seq_char == 'T')
					bucket[ 5 ][ count_T++ ] = vec_value;
				else if(seq_char == 'A')
					bucket[ 1 ][ count_A++ ] = vec_value;
				else if(seq_char == 'G')
					bucket[ 3 ][ count_G++ ] = vec_value;
				else if(seq_char == 'C')
					bucket[ 2 ][ count_C++ ] = vec_value;
				else if(seq_char == 'N')
					bucket[ 4 ][ count_N++ ] = vec_value;
				else if(seq_char == '$')
					bucket[ 0 ][ count_$++ ] = vec_value;
				//else
				//	std::cerr << "FFFUCK: "<<seq_char << std::endl;
			}
			
			/*
			INTTYPE vec_count(begin);
			INTTYPE* ptr = &vec[0];
			if(count_$ != 0)	std::swap_ranges(bucket[0], bucket[0] + count_$, vec.begin()+vec_count );
			//{memcpy( (ptr+vec_count) , bucket[0], count_$ << INTMOVE );
			//vec_count += count_$;
			//}
			if(count_A != 0)	std::swap_ranges(bucket[1], bucket[1] + count_A, vec.begin()+vec_count );
			//{memcpy( (ptr+vec_count) , bucket[1], count_A << INTMOVE );
			//vec_count += count_A;
			//}
			if(count_C != 0)	std::swap_ranges(bucket[2], bucket[2] + count_C, vec.begin()+vec_count );
			//{memcpy( (ptr+vec_count) , bucket[2], count_C << INTMOVE );
			//vec_count += count_C;
			//}
			if(count_G != 0)	std::swap_ranges(bucket[3], bucket[3] + count_G, vec.begin()+vec_count );
			//{memcpy( (ptr+vec_count) , bucket[3], count_G << INTMOVE );
			//vec_count += count_G;
			//}
			if(count_N != 0)	std::swap_ranges(bucket[4], bucket[4] + count_N, vec.begin()+vec_count );
			//memcpy( (ptr+vec_count) , bucket[4], count_N << 2 );
			//vec_count += count_N;
			if(count_T != 0)	std::swap_ranges(bucket[5], bucket[5] + count_T, vec.begin()+vec_count );
			//{memcpy( (ptr+vec_count) , bucket[5], count_T << INTMOVE );
			//vec_count += count_T;
			//}
			*/
			
			
			INTTYPE vec_count(begin);
			if(count_$ != 0)	std::swap_ranges(bucket[0].begin(), bucket[0].begin() + count_$, vec.begin()+vec_count );
			vec_count += count_$;
			if(count_A != 0)	std::swap_ranges(bucket[1].begin(), bucket[1].begin() + count_A, vec.begin()+vec_count );
			vec_count += count_A;
			if(count_C != 0)	std::swap_ranges(bucket[2].begin(), bucket[2].begin() + count_C, vec.begin()+vec_count );
			vec_count += count_C;
			if(count_G != 0)	std::swap_ranges(bucket[3].begin(), bucket[3].begin() + count_G, vec.begin()+vec_count );
			vec_count += count_G;
			if(count_N != 0)	std::swap_ranges(bucket[4].begin(), bucket[4].begin() + count_N, vec.begin()+vec_count );
			vec_count += count_N;
			if(count_T != 0)	std::swap_ranges(bucket[5].begin(), bucket[5].begin() + count_T, vec.begin()+vec_count );
			vec_count += count_T;
			
			
			vec_count = begin;
			if(count_$ != 0)	{bucket_sort(vec_count, vec_count + count_$, depth+1);
			vec_count += count_$;}
			if(count_A != 0)	{bucket_sort(vec_count, vec_count + count_A, depth+1);
			vec_count += count_A;}
			if(count_C != 0)	{bucket_sort(vec_count, vec_count + count_C, depth+1);
			vec_count += count_C;}
			if(count_G != 0)	{bucket_sort(vec_count, vec_count + count_G, depth+1);
			vec_count += count_G;}
			if(count_N != 0)	{bucket_sort(vec_count, vec_count + count_N, depth+1);
			vec_count += count_N;}
			if(count_T != 0)	{bucket_sort(vec_count, vec_count + count_T, depth+1);
			vec_count += count_T;}
			
	}
	
	

private:
	
	typename VECTORTYPE::iterator tx;

	inline INTTYPE real_idx(INTTYPE i, INTTYPE depth)
	{
		return getTableV(vec[tx-vec.begin()+i])+depth;
	}
	inline char i2c(INTTYPE i, INTTYPE depth)
	{
		return seq[ getTableV(vec[tx-vec.begin()+i])+depth ];
	}
	inline void mswap(INTTYPE a, INTTYPE b)
	{
		std::swap(vec[tx-vec.begin()+a],vec[tx-vec.begin()+b]);
	}
	inline void vecmswap(INTTYPE i, INTTYPE j, INTTYPE n)
	{
		std::swap_ranges(tx+i, tx+i+n, tx+j);
	}
	
};





















#endif
