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



int main()
{

	string inner_subject;
	string loop_subject, loop_predicate, loop_object;
	
	
	std::ifstream in("/home/konix/Documents/ED20/BSBS/bsbmtenmil.nt");

	
	
	std::ifstream innerin,subjectivein;
	
	ofs.open ("./bsbm_chain.json", std::ofstream::out | std::ofstream::trunc);
	

//	ofs << "{\n \"docs\": "; //for couchdb
	ofs << "[\n";


	std::string  triple, inner_triple, subjective_triple;
	bool start_of_chain = false;
	bool first_triple = true;
	int chain_length=0;	
	cout << "Please wait..." <<"\n";
	
	while (std::getline(in, triple))
	{
	


		inner_subject   = get_subject(triple);

		//cout << check++ << " "<< inner_subject <<"\n";
		
		//if (inner_subject != "\"" && inner_subject != "\n" && inner_subject != "\'" && inner_subject != "")
		//{

				loop_predicate = get_predicate(triple);
				if ( loop_predicate == "\"page\"")
					homepage_indicator = 'Y';

				loop_object = get_object(triple);
				
				cout << "Subject: " << inner_subject << "  Predicate: " << loop_predicate << "  Objedct:" << loop_object << "\n";
				
				innerin.open("/home/konix/Documents/ED20/BSBS/bsbmtenmil.nt");
				//std::ifstream in("/home/konix/Documents/ED20/Fish/testfish.nt");
				
				while (std::getline(innerin, inner_triple))
				{

					loop_subject = get_subject(inner_triple);

					//cout << "Inner Sub-> "<< check <<"   "<< loop_subject <<"\n";
					
					//cout << loop_subject << "<>" <<inner_subject <<"\n";
					//cout << loop_object  << "==" <<loop_subject  <<"\n";
					//cin.get();
					
					if ( !boost::iequals(loop_subject,inner_subject) &
					      boost::iequals(loop_object ,loop_subject)  )
					{
					
						chain_length++;
						
						if (!start_of_chain)
						{
							start_of_chain = true;
							cout << "start: " << inner_subject << "\n";
							ofs << "{\n";
							ofs << "\t\"start\": " << inner_subject << ",\n";
						
						ofs << "\t" <<loop_predicate << ":" << loop_subject << ",\n" ;
												
						
						//cout << inner_subject << "- " << loop_predicate << " ->" << loop_subject << "\n" ;

						
						}else
						{
							ofs << "\t" <<loop_predicate << ":" << loop_subject << ",\n" ;
				
							//cout << inner_subject << "- " << loop_predicate << " ->" << loop_object << "\n" ;
					
					//start_of_chain = false;					
						}
						
						inner_subject = loop_subject;
						loop_predicate = get_predicate(inner_triple);
						loop_object = get_object(inner_triple);

						
						
											
						
					}
					
				}
				innerin.close();
				if (start_of_chain)
				{
					ofs << "\t" << loop_predicate << ":" << loop_object << ",\n" ;
					
					
					//******************************************************************************************
					//##########################################################################################
					subjectivein.open("/home/konix/Documents/ED20/BSBS/bsbmtenmil.nt");
					

					while (std::getline(subjectivein, subjective_triple))
					{
						loop_subject = get_subject(subjective_triple);
						
						if (boost::iequals(inner_subject ,loop_subject))
						{
							if (first_triple)
							{
								first_triple = false;
							}else{
								loop_predicate = get_predicate(subjective_triple);
								if ( loop_predicate == "\"page\"")
									homepage_indicator = 'Y';
								loop_object = get_object(subjective_triple);
								ofs << "\t" <<loop_predicate << ":" << loop_object << ",\n" ;
							}
							
						}
					}
					subjectivein.close();
					first_triple = true;
					//******************************************************************************************
					//##########################################################################################


					
					
					ofs << "\t\"length\": " << ++chain_length << "\n";								
					//ofs << "\t\"end\": " << loop_object << ",\n";
					//cout << "end: " << loop_object << "\n\n";
					ofs << "},\n";
					start_of_chain = false;					
				}
			
				
				chain_length = 0;	
				
				

				
		//}
		
	}
	
		ofs << "{}]\n";
	//	ofs << "{}]}"; //for couchdb
		ofs.close();
					
		cout << "The file  is ready. :)" <<"\n";				
					
		


return 0;
}



//******************************************************
//
//******************************************************
std::string get_subject(string& triple ){



	unsigned first, last;
	string subject;

	
		first = 0;
		last = triple.find(">");
		subject = triple.substr (0,last);
		triple = triple.substr (last+1,triple.length());
		while (first < subject.length())
		{
			first = subject.find("/");
			subject = subject.substr (first+1, subject.length());
			//cout << subject << endl;
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

		first = 0;
		last = triple.find(">");
		predicate = triple.substr (0,last);
		triple = triple.substr (last+1,triple.length());
		while (first <= predicate.length())
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




























