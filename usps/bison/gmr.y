%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%% /* The ABNF USPS Grammar definition -- EDIT */
rule:
	recipient-sender CRLF
	street-address CRLF
	street-addressss CRLF
	city-town state zip-code CRLF
	country
	;

// Sender
recipient-sender:
	name recipient-senders name
	;

recipient-senders:
	/* empty */
	| name
	;

name:
	ALPHA WSP
	;

names:
	ALPHA | names ALPHA
	;

// Street Line 1
street-address:
	street-addresss street-addressss
	;

street-addresss:
	DIGIT | street-addresss DIGIT
	;

// Street Line 2
street-addressss:
	street-addresssss | street-addressss street-addresssss
	;

street-addresssss:
	WSP
	| ALPHA
	| DIGIT
	;

// City or Town
city-town:
	city-towns WSP | city-town city-towns WSP
	;

city-towns:
	ALPHA | city-towns ALPHA
	;

// state
state:
	state-abbreviation
	| state-full
	;

state-abbreviation:
	"AL" | "AK" | "AZ" | "AR" | "AS" | "CA" | "CO" | "CT" | "DE" | "DC" | "FL" | "GA" | "GU" | "HI" | "ID" | "IL" | "IN" | "IA" | "KS" | "KY" | "LA" | "ME" | "MD" | "MA" | "MI"
	| "MN" | "MS" | "MO" | "MT" | "NE" | "NV" | "NH" | "NJ" | "NM" | "NY" | "NC" | "ND" | "MP" | "OH" | "OK" | "OR" | "PA" | "PR" | "RI" | "SC" | "SD" | "TN" | "TX" | "TT" | "UT"
	| "VT" | "VA" | "VI" | "WA" | "WV" | "WI" | "WY"
	;

state-full:
	"Alabama" | "Alaska" | "Arizona" | "Arkansas" | "California" | "Colorado" | "Connecticut" | "Delaware" | "Florida" | "Georgia" | "Hawaii" | "Idaho"
	| "Illinois" | "Indiana" | "Iowa" | "Kansas" | "Kentucky" | "Louisiana" | "Maine" | "Maryland" | "Massachusetts" | "Michigan" | "Minnesota"
	| "Mississippi" | "Missouri" | "Montana" | "Nebraska" | "Nevada" | "New Hampshire" | "New Jersey" | "New Mexico" | "New York" | "North Carolina"
	| "North Dakota" | "Ohio" | "Oklahoma" | "Oregon" | "Pennsylvania" | "Rhode Island" | "South Carolina" | "South Dakota" | "Tennessee" | "Texas"
	| "Utah" | "Vermont" | "Virginia" | "Washington" | "West Virginia" | "Wisconsin" | "Wyoming"
	;

// postal-code
zip-code:
	DIGIT | zip-code DIGIT
	;

// country
country:
	country-ab
	| country-abbreviation
	| country-full
	;

country-ab:
	"AF" | "AX" | "AL" | "DZ" | "AS" | "AD" | "AO" | "AI" | "AQ" | "AG" | "AR" | "AM" | "AW" | "AU" | "AT" | "AZ" | "BS" | "BH" | "BD" | "BB" | "BY" | "BE" | "BZ" | "BJ" | "BM" | "BT" | "BO" | "BA" | "BW" | "BV" | "BR" | "VG" | "IO" | "BN" | "BG"
	| "BF" | "BI" | "KH" | "CM" | "CA" | "CV" | "KY" | "CF" | "TD" | "CL" | "CN" | "HK" | "MO" | "CX" | "CC" | "CO" | "KM" | "CG" | "CD" | "CK" | "CR" | "CI" | "HR" | "CU" | "CY" | "CZ" | "DK" | "DJ" | "DM" | "DO" | "EC" | "EG" | "SV" | "GQ" | "ER"
	| "EE" | "ET" | "FK" | "FO" | "FJ" | "FI" | "FR" | "GF" | "PF" | "TF" | "GA" | "GM" | "GE" | "DE" | "GH" | "GI" | "GR" | "GL" | "GD" | "GP" | "GU" | "GT" | "GG" | "GN" | "GW" | "GY" | "HT" | "HM" | "VA" | "HN" | "HU" | "IS" | "IN" | "ID" | "IR"
	| "IQ" | "IE" | "IM" | "IL" | "IT" | "JM" | "JP" | "JE" | "JO" | "KZ" | "KE" | "KI" | "KP" | "KR" | "KW" | "KG" | "LA" | "LV" | "LB" | "LS" | "LR" | "LY" | "LI" | "LT" | "LU" | "MK" | "MG" | "MW" | "MY" | "MV" | "ML" | "MT" | "MH" | "MQ" | "MR"
	| "MU" | "YT" | "MX" | "FM" | "MD" | "MC" | "MN" | "ME" | "MS" | "MA" | "MZ" | "MM" | "NA" | "NR" | "NP" | "NL" | "AN" | "NC" | "NZ" | "NI" | "NE" | "NG" | "NU" | "NF" | "MP" | "NO" | "OM" | "PK" | "PW" | "PS" | "PA" | "PG" | "PY" | "PE" | "PH"
	| "PN" | "PL" | "PT" | "PR" | "QA" | "RE" | "RO" | "RU" | "RW" | "BL" | "SH" | "KN" | "LC" | "MF" | "PM" | "VC" | "WS" | "SM" | "ST" | "SA" | "SN" | "RS" | "SC" | "SL" | "SG" | "SK" | "SI" | "SB" | "SO" | "ZA" | "GS" | "SS" | "ES" | "LK" | "SD"
	| "SR" | "SJ" | "SZ" | "SE" | "CH" | "SY" | "TW" | "TJ" | "TZ" | "TH" | "TL" | "TG" | "TK" | "TO" | "TT" | "TN" | "TR" | "TM" | "TC" | "TV" | "UG" | "UA" | "AE" | "GB" | "US" | "UM" | "UY" | "UZ" | "VU" | "VE" | "VN" | "VI" | "WF" | "EH" | "YE"
	| "ZM" | "ZW"
	;


country-abbreviation:
	"AFG" | "ALA" | "ALB" | "DZA" | "ASM" | "AND" | "AGO" | "AIA" | "ATA" | "ATG" | "ARG" | "ARM" | "ABW" | "AUS" | "AUT" | "AZE" | "BHS" | "BHR" | "BGD" | "BRB" | "BLR" | "BEL"
	| "BLZ" | "BEN" | "BMU" | "BTN" | "BOL" | "BIH" | "BWA" | "BVT" | "BRA" | "VGB" | "IOT" | "BRN" | "BGR" | "BFA" | "BDI" | "KHM" | "CMR" | "CAN" | "CPV" | "CYM" | "CAF" | "TCD"
	| "CHL" | "CHN" | "HKG" | "MAC" | "CXR" | "CCK" | "COL" | "COM" | "COG" | "COD" | "COK" | "CRI" | "CIV" | "HRV" | "CUB" | "CYP" | "CZE" | "DNK" | "DJI" | "DMA" | "DOM" | "ECU"
	| "EGY" | "SLV" | "GNQ" | "ERI" | "EST" | "ETH" | "FLK" | "FRO" | "FJI" | "FIN" | "FRA" | "GUF" | "PYF" | "ATF" | "GAB" | "GMB" | "GEO" | "DEU" | "GHA" | "GIB" | "GRC" | "GRL"
	| "GRD" | "GLP" | "GUM" | "GTM" | "GGY" | "GIN" | "GNB" | "GUY" | "HTI" | "HMD" | "VAT" | "HND" | "HUN" | "ISL" | "IND" | "IDN" | "IRN" | "IRQ" | "IRL" | "IMN" | "ISR" | "ITA"
	| "JAM" | "JPN" | "JEY" | "JOR" | "KAZ" | "KEN" | "KIR" | "PRK" | "KOR" | "KWT" | "KGZ" | "LAO" | "LVA" | "LBN" | "LSO" | "LBR" | "LBY" | "LIE" | "LTU" | "LUX" | "MKD" | "MDG"
	| "MWI" | "MYS" | "MDV" | "MLI" | "MLT" | "MHL" | "MTQ" | "MRT" | "MUS" | "MYT" | "MEX" | "FSM" | "MDA" | "MCO" | "MNG" | "MNE" | "MSR" | "MAR" | "MOZ" | "MMR" | "NAM" | "NRU"
	| "NPL" | "NLD" | "ANT" | "NCL" | "NZL" | "NIC" | "NER" | "NGA" | "NIU" | "NFK" | "MNP" | "NOR" | "OMN" | "PAK" | "PLW" | "PSE" | "PAN" | "PNG" | "PRY" | "PER" | "PHL" | "PCN"
	| "POL" | "PRT" | "PRI" | "QAT" | "REU" | "ROU" | "RUS" | "RWA" | "BLM" | "SHN" | "KNA" | "LCA" | "MAF" | "SPM" | "VCT" | "WSM" | "SMR" | "STP" | "SAU" | "SEN" | "SRB" | "SYC"
	| "SLE" | "SGP" | "SVK" | "SVN" | "SLB" | "SOM" | "ZAF" | "SGS" | "SSD" | "ESP" | "LKA" | "SDN" | "SUR" | "SJM" | "SWZ" | "SWE" | "CHE" | "SYR" | "TWN" | "TJK" | "TZA" | "THA"
	| "TLS" | "TGO" | "TKL" | "TON" | "TTO" | "TUN" | "TUR" | "TKM" | "TCA" | "TUV" | "UGA" | "UKR" | "ARE" | "GBR" | "USA" | "UMI" | "URY" | "UZB" | "VUT" | "VEN" | "VNM" | "VIR"
	| "WLF" | "ESH" | "YEM" | "ZMB" | "ZWE"
	;

country-full:
	"Afghanistan" | "Albania" | "Algeria" | "Andorra" | "Angola" | "Antigua" | "Barbuda" | "Argentina" | "Armenia" | "Australia"
	| "Austria" | "Azerbaijan" | "Bahamas" | "Bahrain" | "Bangladesh" | "Barbados" | "Belarus" | "Belgium" | "Belize" | "Benin"
	| "Bhutan" | "Plurinational State of Bolivia" | "Bosnia and Herzegovina" | "Botswana" | "Brazil" | "Brunei Darussalam"
	| "Bulgaria" | "Burkina Faso" | "Burundi" | "Cabo Verde" | "Cambodia" | "Cameroon" | "Canada" | "Central African Republic"
	| "Chad" | "Chile" | "China" | "Colombia" | "Comoros" | "Congo" | "Costa Rica" | "Côte D'Ivoire" | "Croatia" | "Cuba" | "Cyprus"
	| "Czechia" | "Democratic People's Republic of Korea" | "Democratic Republic of the Congo" | "Denmark" | "Djibouti"
	| "Dominica" | "Dominican Republic" | "Ecuador" | "Egypt" | "El Salvador" | "Equatorial Guinea" | "Eritrea" | "Estonia"
	| "Eswatini" | "Ethiopia" | "Fiji" | "Finland" | "France" | "Gabon" | "Republic of The Gambia" | "Georgia" | "Germany" | "Ghana"
	| "Greece" | "Grenada" | "Guatemala" | "Guinea" | "Guinea Bissau" | "Guyana" | "Haiti" | "Honduras" | "Hungary" | "Iceland"
	| "India" | "Indonesia" | "Islamic Republic of Iran" | "Iraq" | "Ireland" | "Israel" | "Italy" | "Jamaica" | "Japan" | "Jordan"
	| "Kazakhstan" | "Kenya" | "Kiribati" | "Kuwait" | "Kyrgyzstan" | "Lao People’s Democratic Republic" | "Latvia" | "Lebanon"
	| "Lesotho" | "Liberia" | "Libya" | "Liechtenstein" | "Lithuania" | "Luxembourg" | "Madagascar" | "Malawi" | "Malaysia"
	| "Maldives" | "Mali" | "Malta" | "Marshall Islands" | "Mauritania" | "Mauritius" | "Mexico" | "Republic of Korea" | "Qatar"
	| "Federated States of Micronesia" | "Monaco" | "Mongolia" | "Montenegro" | "Morocco" | "Mozambique" | "Myanmar" | "Namibia"
	| "Nauru" | "Nepal" | "Netherlands" | "New Zealand" | "Nicaragua" | "Niger" | "Nigeria" | "North Macedonia" | "Norway" | "Oman"
	| "Pakistan" | "Palau" | "Panama" | "Papua New Guinea" | "Paraguay" | "Peru" | "Philippines" | "Poland" | "Portugal"
	| "Republic of Moldova" | "Romania" | "Russian Federation" | "Rwanda" | "Saint Kitts and Nevis" | "Saint Lucia"
	| "Saint Vincent and the Grenadines" | "Samoa" | "San Marino" | "Sao Tome and Principe" | "Saudi Arabia" | "Senegal"
	| "Serbia" | "Seychelles" | "Sierra Leone" | "Singapore" | "Slovakia" | "Slovenia" | "Solomon Islands" | "Somalia"
	| "South Africa" | "South Sudan" | "Spain" | "Sri Lanka" | "Sudan" | "Suriname" | "Sweden" | "Switzerland" | "Syrian Arab Republic"
	| "Tajikistan" | "Thailand" | "Timor-Leste" | "Togo" | "Tonga" | "Trinidad and Tobago" | "Tunisia" | "Türkiye" | "Turkmenistan"
	| "Tuvalu" | "Uganda" | "Ukraine" | "United Arab Emirates" | "United Kingdom of Great Britain and Northern Ireland"
	| "United Republic of Tanzania" | "United States of America" | "Uruguay" | "Uzbekistan" | "Vanuatu"
	| "Bolivarian Republic of Venezuela" | "Viet Nam" | "Yemen" | "Zambia" | "Zimbabwe"
	;

ALPHA:
	'\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' |
	'\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' |
	'\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x61' | '\x62' | '\x63' | '\x64' |
	'\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' |
	'\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' |
	'\x79' | '\x7A'
	;

DIGIT:
	'\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39'
	;

WSP:
	WSPS | WSP WSPS

WSPS:
    SP | HTAB
    ;

HTAB:
	'\x09'
	;

SP:
	'\x20'
	;

CRLF:
	/* empty */
	| CRLF CRLFS
	;

CRLFS:
    CR
	| LF
    ;

CR:
	'\x0D'
	;

LF:
	'\x0A'
	;

%% /* C-Code */
