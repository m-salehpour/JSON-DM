#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <cctype>
#include <algorithm>
#include <boost/algorithm/string.hpp>
//#include <boost/algorithm/string/replace.hpp>





using namespace std;


std::string get_subject(string& triple);
std::string get_predicate(string& triple);
std::string get_object(string& triple);
std::ofstream ofs;
char homepage_indicator = 'N';



//***************************************************
//Struct to facilitate sorting and remove duplications!
//
//***************************************************
struct stc_Triple
{

    string predicate;
    string object;


    //stc_Triple(const string s, const string p, const string p):
      //  subject(s),predicate(p),object(o){}
};

bool SortBy(const stc_Triple & p1, const stc_Triple & p2)
{
    return p1.predicate < p2.predicate || (p1.predicate == p2.predicate && p1.object < p2.object) ;
}

bool Duptriple(const stc_Triple & p1, const stc_Triple & p2)
{
    return ((p1.predicate == p2.predicate) && (p1.object == p2.object));

}

string global_subject;
vector<stc_Triple> VEC_triples_structure;
void add_json_doc();

//**********************

	unsigned long long lineno=0,subno=0;;

int main()
{
	


	string subject, predicate, object;
	



	std::ifstream in("/home/ubuntu/BSBM/tenmil.nt");
	ofs.open ("/home/ubuntu/EDBT/output/tenbsbmSNV.json", std::ofstream::out | std::ofstream::trunc);
	

//	ofs << "{\n \"docs\": "; //for couchdb
	ofs << "[\n";


	std::string  triple;
	char subchange = 'f';
	string subtmp = "empty";
	
	while (std::getline(in, triple))
	{

		subject   = get_subject(triple);
		if (subject != "\"" && subject != "\n" && subject != "\'" && subject != "")
		{
			if (subtmp == "empty")
				subtmp = subject;
				
			else if (!boost::iequals(subject,subtmp))
			{
				subchange = 't';
				global_subject = subtmp;
				subtmp = subject;
			}
				
			if (subchange == 'f')
			{			
				predicate = get_predicate(triple);
				if ( predicate == "\"page\"")
				homepage_indicator = 'Y';
					
				object = get_object(triple);
		
				VEC_triples_structure.push_back({predicate,object});

			}else
			{
			subchange = 'f';
			
			/*	
			cout << "\n\n******************Before SORT******************\n\n";
			cout << "\n SIZE:\t" << VEC_triples_structure.size() << "\n";
			cout << "\n\n******************START******************\n\n";
			for (size_t i=0; i < VEC_triples_structure.size();i++)
				cout << lineno++ << "\t" << global_subject << "\t" <<VEC_triples_structure[i].predicate << "\t" <<VEC_triples_structure[i].object << "\n";
			cout << "\n\n******************END******************\n\n";
			*/


			sort(VEC_triples_structure.begin(), VEC_triples_structure.end(), SortBy);
			auto last_structs = std::unique(VEC_triples_structure.begin(), VEC_triples_structure.end(), Duptriple);
			VEC_triples_structure.erase(last_structs,VEC_triples_structure.end());
		
			/*
			cout << "\n SIZE:\t" << VEC_triples_structure.size() << "\n";
			cout << "\n\n###############START###################\n\n";
			for (size_t i=0; i < VEC_triples_structure.size();i++)
				cout << lineno++ << "\t" << global_subject << "\t" <<VEC_triples_structure[i].predicate << "\t" <<VEC_triples_structure[i].object << "\n";
			cout << "\n\n###############END###################\n\n";
			*/
			
			if (VEC_triples_structure.size() > 0)
			{
				add_json_doc();
	  			VEC_triples_structure.clear();
				vector<stc_Triple>().swap(VEC_triples_structure);
			}
				
				
			}
		}//if for not nul subject	
		
	}	
	
	global_subject = subtmp;
	sort(VEC_triples_structure.begin(), VEC_triples_structure.end(), SortBy);
	auto last_structs = std::unique(VEC_triples_structure.begin(), VEC_triples_structure.end(), Duptriple);
	VEC_triples_structure.erase(last_structs,VEC_triples_structure.end());

	//for (size_t i=0; i < VEC_triples_structure.size();i++)
	//	cout << lineno++ << "\t" << global_subject << "\t" <<VEC_triples_structure[i].predicate << "\t" <<VEC_triples_structure[i].object << "\n";


	if (VEC_triples_structure.size() > 0)
	{			
		add_json_doc();
  		VEC_triples_structure.clear();
		vector<stc_Triple>().swap(VEC_triples_structure);
	}

		
		

	ofs << "\n";
	ofs << "{}]";
//	ofs << "{}]}"; //for couchdb
	ofs.close();
		
		


return 0;
}
//******************************************************
//
//******************************************************

void add_json_doc(){
	//std::ofstream ofs;
	//ofs.open ("./ISWC/iswc.json", std::ofstream::out | std::ofstream::app);

	int i;
	std::string temp_top_predicate;
	
	//if (global_subject.length() > 23)
	//	global_subject = global_subject.substr(0,20) + "\"";
 	
	
	ofs << "{\n";
	//ofs << "\t\"_id\":" << global_subject << ",\n";
	ofs << "\t\"idPrimaryKey\":" << global_subject << ",\n";
	
	cout << "\n\n" << subno++ << ":\t"<<global_subject << ":\n" << "------------------------------------------------------------------------------------\n";
	cout << "Triple No." << ":\t" << "\t" << "Predicate" << "\t" <<"Object" << "\n";


	i = 0;		
	while ( (VEC_triples_structure.size()-1) > i)
	{



			
			temp_top_predicate = VEC_triples_structure[i].predicate;
			
			if (VEC_triples_structure[i+1].predicate != temp_top_predicate)
			{
								
				ofs << "\t" << temp_top_predicate << ":" << VEC_triples_structure[i].object;
				
				cout << lineno++ << ":\t" << "\t" << temp_top_predicate << "\t" <<VEC_triples_structure[i].object << "\n";
				
				if (VEC_triples_structure.size() != (i+1))
					ofs << ",\n";
				else
					ofs << "\n";

			}else
			{
				
			
			ofs << "\t" << temp_top_predicate << ":[";
			ofs <<  VEC_triples_structure[i].object << ",";

			cout << lineno++ << ":\t" << "\t" << temp_top_predicate << "\t" <<VEC_triples_structure[i].object << "\n";
			
			i++;
			temp_top_predicate = VEC_triples_structure[i].predicate;
	
 				
 				
					if ((VEC_triples_structure.size()-1) >= (i+1))
					{
						
						while ( VEC_triples_structure[i+1].predicate == temp_top_predicate) 
						{
					
							
							ofs <<  VEC_triples_structure[i].object << ",";

							cout << lineno++ << ":\t" << "\t" << temp_top_predicate << "\t" <<VEC_triples_structure[i].object << "\n";
							
							i++;

							
		 					if ((VEC_triples_structure.size()-1) <= i)	
		 						break;
		 						
		 					
		 					
 							temp_top_predicate = VEC_triples_structure[i].predicate;
		 			
						}
					}



			
					
							
			ofs <<  VEC_triples_structure[i].object << "]";
			cout << lineno++ << ":\t" << "\t" << temp_top_predicate << "\t" <<VEC_triples_structure[i].object << "\n";
			
			if (VEC_triples_structure.size() != (i+1))
				ofs << ",\n";
			else
				ofs << "\n";


	



			}
			i++;
			
			
	}

	if (VEC_triples_structure.size() == (i+1))
	{
		ofs << "\t" << VEC_triples_structure[i].predicate << ":" << VEC_triples_structure[i].object << "\n";
		cout << lineno++ << ":\t" << "\t" << VEC_triples_structure[i].predicate << "\t" <<VEC_triples_structure[i].object << "\n";
	}
	ofs << "},\n";


}//add_json_doc






//******************************************************
//
//******************************************************
std::string get_subject(string& triple ){



	unsigned first, last;
	string subject;
	static const size_t npos = -1;
	
		first = 0;
		last = triple.find(">");
		subject = triple.substr (0,last);
		triple = triple.substr (last+1,triple.length());
		while ( subject.find("/") != string::npos )
		{
		
			first = subject.find("/");

			//cout << "*****First: " << first << "  Length:  " << subject.length() << " Subject:  " << subject << endl;
			
			subject = subject.substr (first+1, subject.length());

			//cout << "After ####### Length:  " << subject.length() << " Subject:  " << subject << endl;
			
			
			
			
		}

/*
		subject.erase(std::remove(subject.begin(), subject.end(), '/'), subject.end());				
		subject.erase(std::remove(subject.begin(), subject.end(), '\"'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '\''), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '\\'), subject.end());				
		subject.erase(std::remove(subject.begin(), subject.end(), ';'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '.'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '$'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '*'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '<'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '>'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), ':'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '|'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '?'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '_'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '%'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '-'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '!'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '~'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '#'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '^'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '&'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '('), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), ')'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '+'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '='), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '['), subject.end());				
		subject.erase(std::remove(subject.begin(), subject.end(), ']'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '{'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '}'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), ','), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '\t'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), ' '), subject.end());													
*/		
		
						
		
		subject = "\"" + subject + "\"";

return subject;
}



//******************************************************
//
//******************************************************
std::string get_predicate(string& triple ){



	unsigned first, last;
	string predicate;
		static const size_t npos = -1;

		first = 0;
		last = triple.find(">");
		predicate = triple.substr (0,last);
		triple = triple.substr (last+1,triple.length());
		while (predicate.find("/") != string::npos)
		{
			first = predicate.find("/");
			predicate = predicate.substr (first+1, predicate.length());
			//cout << subject << endl;
		}


		if ( predicate.find("#") < predicate.length() )
			predicate = predicate.substr ((predicate.find("#"))+1, predicate.length());

		predicate = "\"" + predicate + "\"";
		//cout << predicate << endl;

	
return predicate;
}



//******************************************************
//
//******************************************************



bool is_number(string str){

	for(size_t i=0; i < str.length() ; i++)
	{
		if (!isdigit( str[ i ] ))
			return false;
	}

return true;
}//is_number










//******************************************************
//
//******************************************************
std::string get_object(string& triple){


	unsigned first, last;
	string object;
	char flag = 't';


		first = 0;
		

		if (homepage_indicator == 'Y')
		{
			first = triple.find("<");
			last = triple.find(">");
			object = triple.substr (first+1, last-3);
			//object = "\"" + object + "\"";
			homepage_indicator = 'N';


		}else if ( (triple.find(">") < triple.length())  && (triple.find("^") > triple.length()) )
		{
			object = triple.substr (0,triple.find(">"));
			if (object.substr(object.length()-2,object.length()).find("/") <= object.length())
				object = triple.substr (0,triple.find(">")-1);


			while (first < object.length())
			{
				first = object.find("/");
				object = object.substr (first+1, object.length());
				//cout << subject << endl;
			}
				if ( object.find("#") <= object.length())
				{
					first = object.find("#");
					object = object.substr (first+1, object.length());
				}

				//object = "\"" + object + "\"";
		}
		else if (triple.find(">") < triple.length()  && (triple.find("^") < triple.length()) )
		{
			//flag = 'f';
			
			first = triple.find("\"");			
			last  = triple.substr (first+1,triple.length()).find("\"");



			if (is_number(triple.substr (first+1,last)) || ( triple.substr (first+1,last).find(".") <= triple.substr (first+1,last).length() &&
			triple.substr (first+1,last).find(":") > triple.substr (first+1,last).length() )) 
			{
				object = triple.substr (first+1,last);
				//cout << "number-->" << object << "\n";
				flag = 'f';

			}else if ( triple.substr (first+1,last).find("-") <= triple.substr (first+1,last).length() )  
			{
				object = triple.substr (first+1,last);

		

					if (object.find("T") <= object.length())
						object = object.substr (0,object.find("T"));// + "\"";

					//object = "{\"$date\":\"" + object + "T00:00:00.000Z\"}";  //ISODate("2013-10-01T00:00:00.000Z"),
					//object = "ISODate(\"" + object + "T00:00:00.000Z\")";  //ISODate("2013-10-01T00:00:00.000Z"),
					//object = "\"" + object + "T00:00:00.000Z\"";  //ISODate("2013-10-01T00:00:00.000Z"),
					//object = "\"" + object + "\"";
			
				
			
			}else
			{
				
				object = triple.substr (first,last+2);			
				
			
			}

		
			

			
			


			

		}else
		{
			first = triple.find("\"");
			object = triple.substr (first,triple.length()-7) +
				 triple.substr (triple.length()-5,triple.length())+
				 "\""; 
			
			

		}
		



	
	if (flag != 'f')
	{
		object.erase(std::remove(object.begin(), object.end(), '\n'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\a'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\b'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\t'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\v'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\f'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\r'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\x1B'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\0'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\00'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\000'), object.end());
		
		object.erase(std::remove(object.begin(), object.end(), '\"'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '('), object.end());
		object.erase(std::remove(object.begin(), object.end(), ')'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '?'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\''), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\\'), object.end());
		
		//boost::replace_all(name, "\"", "\\\"");			
		
		object.erase(std::remove(object.begin(), object.end(), '*'), object.end());				
		object.erase(std::remove(object.begin(), object.end(), '#'), object.end());						
		object.erase(std::remove(object.begin(), object.end(), ';'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '%'), object.end());
		
		object = "\"" + object + "\"";
	
	}


return object;
}













