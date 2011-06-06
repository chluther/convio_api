$Id: 

Description
===========
The Convio API module implements the Convio Open APIs to integrate Convio's
back-end services with Drupal. The module was initally developed by Chris Luther
of AgileID, LLC, an Authorized Convio Solutions Provider.  

For more information about Convio Open APIs please
please see http://open.convio.com/ .   

For more information about AgileID please see
http://www.agileID.com .

This module requires that you have a functioning Convio installation that has
has been configured for API access.  For more information about obtaining
a Convio installation please contact AgileID (info@agileID.com) or Convio
(info@Convio.com).

Additionally, this module is dependent upon TOKENS the module.  
For more information on tokens please see http://drupal.org/project/token


Getting Started
============
1. Configure your Convio site for Convio Open API access.
   see http://open.convio.com/api/#main.site_configuration.html

2. Install and enable the Token module.

3. Enable the Profile module.

4. Install and enable the Convio API module.

5. Review the custom.inc file.  This file maps Convio fields to 
   Drupal user fields.  By default this mapping matches a default 
   Convio configuration.  Verify the mapping against your Convio
   installation and, as necessary, modify the mapping to match 
   your Convio instance configuration.  For additional information 
   on the mapping format please see http://drupal.org/node/307140

6. Go to http://<site>/admin/user/profile to create matching 
   Drupal user fields according to the sample shown below. 
   NOTE: Future versions of this module will automate this process. 
   


7. 


SAMPLE FIELD MAPPING
    Convio Field            Drupal Field                Group   Type
    accepts_postal_mail     convio_accepts_postal_mail  Contact textfield
    preferred_phone         convio_preferred_phone      Contact textfield
    home_phone              convio_home_phone           Contact textfield
    mobile_phone            convio_mobile_phone         Contact textfield
    work_phone              convio_work_phone           Contact textfield
    primary_address         convio_primary_address      Contact textfield
    home_street1            convio_home_street1         Contact textfield
    home_street2            convio_home_street2         Contact textfield
    home_city               convio_home_city            Contact textfield
    home_zip                convio_home_zip             Contact textfield
    home_country            convio_home_country         Contact textfield
    user_name               convio_user_name            Convio  textfield
    cons_id                 convio_cons_id              Convio  textfield
    number_id               convio_number_id            Convio  textfield
    name.title              convio_name_title           Convio  textfield
    name.first              convio_name_first           Convio  textfield
    name.middle             convio_name_middle          Convio  textfield
    name.last               convio_name_last            Convio  textfield
    name.suffix             convio_name_suffix          Convio  textfield
    name.prof_suffix        convio_name_prof_suffix     Convio  textfield
    interest_label          convio_interest_label       Convio  selection
