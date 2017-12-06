#ifndef DMERA_H
#define DMERA_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <utility>

#include <uni10.hpp>

const auto RCTYPE = uni10::RTYPE;

const std::string SVD_FNAME("NetworkSheets/SVD");
const std::string DMS_0_FNAME("NetworkSheets/DMS_0");
const std::string DMS_1_FNAME("NetworkSheets/DMS_1");
const std::string PNAME("NetworkSheets/_");

const bool FROM_FILE = true;

const double DELTA = 0.5;

const int SITES = 12;
const int EPOCH = 50;
const int VAR_TIME = 5;

class Dmera
{
	public:
		Dmera(std::vector<double>, double);

//		std::string summary(bool) const;

		void VarUpdate();

		void check() const;

		static uni10::UniTensor eigenshift(uni10::UniTensor, double&);
		static uni10::UniTensor svdSolveMinimal(uni10::UniTensor, double&);

	private:
    	class Block
		{
			public:
				Block(int);
				int get_idx() const;
				uni10::UniTensor tensor(std::string) const;

				void update(std::string, uni10::UniTensor);

				std::map<std::string, uni10::UniTensor> t;
				std::array<uni10::UniTensor, 3> dm; // density matrix
				std::array<uni10::UniTensor, 5> eh; // effective hamiltonian
				std::array<uni10::UniTensor, 5> ehs; // effective hamiltonian - shift
				std::array<double, 5> es;
			private:
				const int idx;
		};

		class NetworkForm
		{
			public:
				NetworkForm();
				NetworkForm(std::string, int, int, int);
				uni10::UniTensor launch(Block*) const;

			private:
				std::vector<std::string> symbols; //symbol used in block
				uni10::Network* network;
		};

		void BuildNetworkForms();

		// Utils
		static double effective_j(double, double, double, double);
		static int index(int, int);
		static uni10::UniTensor Random_Unitary();
		static uni10::UniTensor Singlet();
		static uni10::UniTensor Identity();
		static uni10::UniTensor DmSinglet(int);
		static uni10::UniTensor TwoSiteHam(double, double);
        
		const int width;

		std::vector<Block*> blocks;
		std::map<std::string, std::vector<NetworkForm>> network;

		std::vector<uni10::UniTensor> dm_init;
		std::vector<uni10::UniTensor> eh_init;

		std::array<double, SITES> es;

		std::ofstream of;
};

// for debug
void print(const uni10::UniTensor&);

#endif //ifndef DMERA_H
