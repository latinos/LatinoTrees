#
# cross section DB
#
# units in pb
#
# References			
#	A	https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSectionsat13TeV		
#	B	https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO		
#	C	https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt1314TeV		
#	D	https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SingleTopRefXsec		
#	E	https://twiki.cern.ch/twiki/bin/view/CMS/SummaryTable1G25ns		
#	F	https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR3		
#	G	https://twiki.cern.ch/twiki/bin/view/CMS/GenXsecTaskForce		
#	H	http://arxiv.org/pdf/1307.7403v1.pdf		
#	I	https://twiki.cern.ch/twiki/bin/viewauth/CMS/HowToGenXSecAnalyzer		
#	J	http://cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=AN2015_099_v8.pdf
#	K	https://twiki.cern.ch/twiki/bin/view/CMS/HiggsWW13TeVProductionMassScan (powheg numbers)
#	L	https://twiki.cern.ch/twiki/bin/view/CMS/HiggsWW13TeVProduction (powheg numbers)
#	X	Unknown! - Cross section not yet there
#
#	
	

samples['WJetsToLNu']                	= ['xsec=61526.7',     'kfact=1.000',		'ref=E']

samples['GluGluHToWWTo2L2Nu_M125']   	= ['xsec=0.9913',      'kfact=1.000',		'ref=CF'] # 43.92*0.215*0.108*0.108*9

# WW 
samples['WWTo2L2Nu']	             	= ['xsec=12.178',     'kfact=1.000',		'ref=E']		
samples['WWToLNuQQ']	             	= ['xsec=49.997',     'kfact=1.000',		'ref=E']	
samples['WWToLNuQQext']	             	= ['xsec=49.997',     'kfact=1.000',		'ref=E']
samples['WWTo4Q'] 	             	= ['xsec=51.723',     'kfact=1.000',		'ref=E']
samples['GluGluWWTo2L2Nu_MCFM']      	= ['xsec=7.948',      'kfact=1.000',		'ref=E'] 
samples['GluGluWWTo2L2NuHiggs_MCFM'] 	= ['xsec=7.948',      'kfact=1.000',		'ref=X'] # Under discussion

# tt
samples['TTTo2L2Nu'] 	             	= ['xsec=87.31',     	'kfact=1.000',		'ref=B']		
	
# ggH,HWW
samples['GluGluHToWWTo2L2Nu_Mlarge']  	= ['xsec=0.6818',     	'kfact=1.000',		'ref=X'] # 10GeV Higgs width 30.21(powheg)*0.215*0.108*0.108*9	

samples['GluGluHToWWTo2L2Nu_alternative_M120']  = ['xsec=0.9818',     	'kfact=1.000',		'ref=CF'] 
samples['GluGluHToWWTo2L2Nu_alternative_M125']  = ['xsec=0.9913',      	'kfact=1.000',		'ref=CF'] 
samples['GluGluHToWWTo2L2Nu_alternative_M130']  = ['xsec=0.9081',     	'kfact=1.000',		'ref=X'] # 28.55(powheg)*0.303*0.108*0.108*9

samples['GluGluHToWWTo2L2Nu_M120']  	= ['xsec=0.4768',     	'kfact=1.000',		'ref=KF'] # 32.21*0.141*0.108*0.108*9  
samples['GluGluHToWWTo2L2Nu_M124']      = ['xsec=0.6353',     	'kfact=1.000',		'ref=KF'] # 30.41*0.199*0.108*0.108*9  
samples['GluGluHToWWTo2L2Nu_M125']      = ['xsec=0.9913',      	'kfact=1.000',		'ref=CF'] # 43.92*0.215*0.108*0.108*9 
samples['GluGluHToWWTo2L2Nu_M126']      = ['xsec=0.7171',     	'kfact=1.000',		'ref=KF'] # 29.57*0.231*0.108*0.108*9      
samples['GluGluHToWWTo2L2Nu_M130']      = ['xsec=0.8906',     	'kfact=1.000',		'ref=KF'] # 28.00*0.303*0.108*0.108*9       
samples['GluGluHToWWTo2L2Nu_M135']      = ['xsec=1.1006',     	'kfact=1.000',		'ref=KF'] # 26.21*0.400*0.108*0.108*9       
samples['GluGluHToWWTo2L2Nu_M140']      = ['xsec=1.2938',     	'kfact=1.000',		'ref=KF'] # 24.60*0.501*0.108*0.108*9        
samples['GluGluHToWWTo2L2Nu_M145']      = ['xsec=1.4594',     	'kfact=1.000',		'ref=KF'] # 23.17*0.600*0.108*0.108*9      
samples['GluGluHToWWTo2L2Nu_M150']      = ['xsec=1.6001',     	'kfact=1.000',		'ref=KF'] # 21.90*0.696*0.108*0.108*9        
samples['GluGluHToWWTo2L2Nu_M155']      = ['xsec=1.7409',     	'kfact=1.000',		'ref=KF'] # 20.86*0.795*0.108*0.108*9    
samples['GluGluHToWWTo2L2Nu_M160']      = ['xsec=1.9102',     	'kfact=1.000',		'ref=KF'] # 20.04*0.908*0.108*0.108*9      
samples['GluGluHToWWTo2L2Nu_M165']      = ['xsec=1.8966',     	'kfact=1.000',		'ref=KF'] # 18.82*0.960*0.108*0.108*9  
samples['GluGluHToWWTo2L2Nu_M170']      = ['xsec=1.8104',     	'kfact=1.000',		'ref=KF'] # 17.89*0.964*0.108*0.108*9    
samples['GluGluHToWWTo2L2Nu_M175']      = ['xsec=1.7147',     	'kfact=1.000',		'ref=KF'] # 17.05*0.958*0.108*0.108*9       
samples['GluGluHToWWTo2L2Nu_M190']      = ['xsec=1.2187',     	'kfact=1.000',		'ref=KF'] # 14.77*0.786*0.108*0.108*9          
samples['GluGluHToWWTo2L2Nu_M200']      = ['xsec=1.0548',     	'kfact=1.000',		'ref=KF'] # 13.56*0.741*0.108*0.108*9            
samples['GluGluHToWWTo2L2Nu_M210']      = ['xsec=0.9639',     	'kfact=1.000',		'ref=KF'] # 12.70*0.723*0.108*0.108*9           
samples['GluGluHToWWTo2L2Nu_M230']      = ['xsec=0.8332',     	'kfact=1.000',		'ref=KF'] # 11.21*0.708*0.108*0.108*9              
samples['GluGluHToWWTo2L2Nu_M250']      = ['xsec=0.7270',     	'kfact=1.000',		'ref=KF'] # 9.88*0.701*0.108*0.108*9   
samples['GluGluHToWWTo2L2Nu_M300']      = ['xsec=0.5731',     	'kfact=1.000',		'ref=KF'] # 7.89*0.692*0.108*0.108*9    
samples['GluGluHToWWTo2L2Nu_M350']      = ['xsec=0.5698',     	'kfact=1.000',		'ref=KF'] # 8.03*0.676*0.108*0.108*9      
samples['GluGluHToWWTo2L2Nu_M400']      = ['xsec=0.4360',     	'kfact=1.000',		'ref=KF'] # 7.136*0.582*0.108*0.108*9   
samples['GluGluHToWWTo2L2Nu_M450']      = ['xsec=0.2926',     	'kfact=1.000',		'ref=KF'] # 5.059*0.551*0.108*0.108*9     
samples['GluGluHToWWTo2L2Nu_M500']      = ['xsec=0.1926',     	'kfact=1.000',		'ref=KF'] # 3.360*0.546*0.108*0.108*9    
samples['GluGluHToWWTo2L2Nu_M600']      = ['xsec=0.8599',     	'kfact=1.000',		'ref=KF'] # 1.468*0.558*0.108*0.108*9      
samples['GluGluHToWWTo2L2Nu_M650']      = ['xsec=0.0593',     	'kfact=1.000',		'ref=KF'] # 0.994*0.568*0.108*0.108*9  
samples['GluGluHToWWTo2L2Nu_M700']      = ['xsec=0.0416',     	'kfact=1.000',		'ref=KF'] # 0.687*0.577*0.108*0.108*9    
samples['GluGluHToWWTo2L2Nu_M800']      = ['xsec=0.0218',     	'kfact=1.000',		'ref=KF'] # 0.3493*0.594*0.108*0.108*9     
samples['GluGluHToWWTo2L2Nu_M900']      = ['xsec=0.0123',     	'kfact=1.000',		'ref=KF'] # 0.1920*0.609*0.108*0.108*9     
samples['GluGluHToWWTo2L2Nu_M1000']     = ['xsec=0.0073',     	'kfact=1.000',		'ref=KF'] # 0.1128*0.621*0.108*0.108*9        

samples['GluGluHToWWTo2L2NuAMCNLO_M125']  = ['xsec=0.9913',      	'kfact=1.000',		'ref=CF'] 
samples['GluGluHToWWTo2L2NuPowheg_M125']  = ['xsec=0.9913',      	'kfact=1.000',		'ref=CF'] 

samples['GluGluHToWWToLNuQQ_M650']      = ['xsec=0.1259',     	'kfact=1.000',		'ref=LF'] # 1.0149*0.568*0.108*3*0.6741     
samples['GluGluHToWWToLNuQQ_M1000']     = ['xsec=0.0029',     	'kfact=1.000',		'ref=LF'] # 0.11877*0.1128*0.108*3*0.6741     


# W-H
samples['HWminusJ_HToWW_M120']       	= ['xsec=0.0866',     	'kfact=1.000',		'ref=EF'] # 0.614 * 0.141
samples['HWminusJ_HToWW_M125']  	= ['xsec=0.1160',     	'kfact=1.000',		'ref=EF'] # 0.539 * 0.215
samples['HWminusJ_HToWW_M130'] 		= ['xsec=0.1430',     	'kfact=1.000',		'ref=EF'] # 0.472* 0.303

# W+H
samples['HWplusJ_HToWW_M120']          	= ['xsec=0.1350',       'kfact=1.000',	       	'ref=EF'] # 0.956 * 0.141
samples['HWplusJ_HToWW_M125']          	= ['xsec=0.1810',       'kfact=1.000',	       	'ref=EF'] # 0.842 * 0.215
samples['HWplusJ_HToWW_M130']          	= ['xsec=0.2250',       'kfact=1.000',	       	'ref=EF'] # 0.743 * 0.303

# ZH
samples['HZJ_HToWW_M120']  	       	= ['xsec=0.121',       	'kfact=1.000',	      	'ref=EF'] # 0.855 * 0.141
samples['HZJ_HToWW_M125']  	       	= ['xsec=0.187',       	'kfact=1.000',	      	'ref=EF'] # 0.8696* 0.215
samples['HZJ_HToWW_M130']  	       	= ['xsec=0.202',       	'kfact=1.000',	      	'ref=EF'] # 0.667* 0.303








