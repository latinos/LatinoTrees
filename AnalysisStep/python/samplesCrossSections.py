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
#	X	Unknown! - Cross section not yet there
#
#	
	

samples['WJetsToLNu']                	.extend( ['xsec=61526.7',     'kfact=1.000',		'ref=E'] )

samples['GluGluHToWWTo2L2Nu_M125']   	.extend( ['xsec=0.9913',      'kfact=1.000',		'ref=CF'] ) # 43.92*0.215*0.108*0.108*9

#WW 
samples['WWTo2L2Nu']	             	.extend( ['xsec= 12.178',     'kfact=1.000',		'ref=E'] )		
samples['WWToLNuQQ']	             	.extend( ['xsec= 49.997',     'kfact=1.000',		'ref=E'] )	
samples['WWToLNuQQext']	             	.extend( ['xsec= 49.997',     'kfact=1.000',		'ref=E'] )
samples['WWTo4Q'] 	             	.extend( ['xsec= 51.723',     'kfact=1.000',		'ref=E'] )
samples['GluGluWWTo2L2Nu_MCFM']      	.extend( ['xsec= 7.948',      'kfact=1.000',		'ref=E'] )
samples['GluGluWWTo2L2NuHiggs_MCFM'] 	.extend( ['xsec= 7.948',      'kfact=1.000',		'ref=X'] ) # Under discussion

#tt
samples['TTTo2L2Nu'] 	             	.extend( ['xsec= 87.31',     	'kfact=1.000',		'ref=B'] )		
	
#HWW
samples['GluGluHToWWTo2L2Nu_Mlarge']  	.extend( ['xsec= 0.6818',     	'kfact=1.000',		'ref=X'] ) # 10GeV Higgs width 30.21*0.215*0.108*0.108*9	
