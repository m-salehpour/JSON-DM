#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <cctype>
#include <algorithm>
#include <boost/algorithm/string.hpp>



	using namespace std;

	std::string get_subject(string& triple);
	std::string get_predicate(string& triple);
	std::string get_object(string& triple);
	
	char homepage_indicator = 'N';

	std::ofstream ofs;

	struct stc_Triple
	{
	string subject;
	string predicate;
	string object;

	stc_Triple(const string ss):
        subject(ss){}
        
    stc_Triple(const string s, const string p, const string o):
       subject(s),predicate(p),object(o){}
       
       bool operator<(const stc_Triple & other) const
      {
        //return ((subject < other.subject)  && (subject.length() < other.subject.length()) ) ;
        return subject < other.subject;
      }
	};
	
	bool SortBySubject(const stc_Triple & p1, const stc_Triple & p2)
	{
	    return p1.subject < p2.subject || (boost::iequals( p1.subject,p2.subject) && p1.predicate < p2.predicate) || (boost::iequals( p1.subject,p2.subject) && boost::iequals( p1.predicate,p2.predicate) && p1.object < p2.object) ;
	}



int main()
{





	vector<stc_Triple> ram_triple;

	string inner_subject;
	string loop_subject, loop_predicate, loop_object;
	
	
	std::ifstream in("/home/konix/Documents/ED20/BSBS/bsbmtenmil.nt");
	//std::ifstream in("/home/konix/Documents/ED20/Fish/fishtest.nt");

	
	std::ifstream innerin,subjectivein;
	
	ofs.open ("/home/konix/Documents/ED20/Parser/output/ram_bsbm_chain.json", std::ofstream::out | std::ofstream::trunc);
	

//	ofs << "{\n \"docs\": "; //for couchdb
	ofs << "[\n";


	std::string  triple, inner_triple, subjective_triple;
	bool start_of_chain = false;
	bool first_triple = true;
	int chain_length=0;	
	cout << "Please wait..." <<"\n";

	std::vector<stc_Triple>::iterator low,up;

	while (std::getline(in, triple))
	{
	


		inner_subject   = get_subject(triple);
		loop_predicate = get_predicate(triple);
		if ( loop_predicate == "\"page\""  || loop_predicate == "\"offerwebpage\"" || loop_predicate == "\"homepage\"")
			homepage_indicator = 'Y';
		loop_object = get_object(triple);
	
		ram_triple.push_back(stc_Triple(inner_subject, loop_predicate, loop_object));
	
		//cout << "Subject: " << inner_subject << "  Predicate: " << loop_predicate << "  Objedct:" << loop_object << "\n";
	
	}
	in.close();

	cout << "Moved to RAM..." <<"\n";
	
	std::sort(ram_triple.begin(), ram_triple.end(), SortBySubject);
	
	cout << "Sort is finished..." <<"\n";	
	
	if(ram_triple.empty())
	        cout << "\nSorry, no values exist in the database.";
	else
        	for (long long triple_index = 0; triple_index < ram_triple.size(); triple_index++)
        	    {       
			
			inner_subject   = ram_triple[triple_index].subject;
			loop_predicate  = ram_triple[triple_index].predicate;
			loop_object     = ram_triple[triple_index].object;
			cout << "------------------------------------------------------" << "\n";	
			cout << "The following triple is undergo:  " << triple_index << "\n";	
			
       		           //unsigned long long i=0;
       		           //low = ram_triple.begin();
       		           
       		           low = std::lower_bound(ram_triple.begin() , ram_triple.end(),stc_Triple(loop_object));
       		           loop_subject =	ram_triple[low - ram_triple.begin()].subject;
       		           
       		           //!boost::iequals(loop_subject,inner_subject) &&
			   while ( boost::iequals(loop_object ,loop_subject) ) //low != ram_triple.end()) //i < ram_triple.size())
			    {
			 	
				cout << "found:  " << loop_object << "at:  " << low - ram_triple.begin() << "\n";	
									
						chain_length++;
						
						if (!start_of_chain)
						{
							start_of_chain = true;
							//cout << "start: " << inner_subject << "\n";
							ofs << "{\n";
							ofs << "\t\"start\": " << inner_subject << ",\n";
							//cout << "\t\"start\": " << inner_subject << ",\n";
							//cout << "position of start:  " << (low - ram_triple.begin()) << "\n";	
						ofs << "\t" <<loop_predicate << ":" << loop_subject << ",\n" ;
												
						
						//cout << inner_subject << "- " << loop_predicate << " ->" << loop_subject << "\n" ;

						
						}else
						{
							ofs << "\t" <<loop_predicate << ":" << loop_subject << ",\n" ;
							//cout << inner_subject << "- " << loop_predicate << " ->" << loop_object << "\n" ;
					
							//start_of_chain = false;					
						}
						
						inner_subject  = loop_subject;
						loop_predicate = ram_triple[low - ram_triple.begin()].predicate;
						loop_object = ram_triple[low - ram_triple.begin()].object;
						
						
						low = std::lower_bound(ram_triple.begin() , ram_triple.end(),stc_Triple(loop_object));

						loop_subject =	ram_triple[low - ram_triple.begin()].subject;      
						
						
					

 
 
  			  }//while
 
 
       				if (start_of_chain)
				{
					
					low = std::lower_bound(ram_triple.begin() , ram_triple.end(),stc_Triple(inner_subject));
					up  = std::upper_bound(ram_triple.begin() , ram_triple.end(),stc_Triple(inner_subject));

				
						while(low != up)
						{
														
							ofs << "\t" << ram_triple[low - ram_triple.begin()].predicate << ":" << ram_triple[low - ram_triple.begin()].object << ",\n" ;
							low++;				
						}
					
					
					ofs << "\t\"length\": " << ++chain_length << "\n";								
					//ofs << "\t\"end\": " << loop_object << ",\n";
					//cout << "end: " << loop_object << "\n\n";
					ofs << "},\n";
					start_of_chain = false;					
				}
			
				
				chain_length = 0;	
				
				
				
				
       
           
       
        	    
        	    
        	    
        	    
        	    
        	    } // main loop
        
	

		ofs << "{}]\n";
	//	ofs << "{}]}"; //for couchdb
		ofs.close();
					
		cout << "The JSON file is ready. :)" <<"\n";				
					
	


return 0;
}




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













