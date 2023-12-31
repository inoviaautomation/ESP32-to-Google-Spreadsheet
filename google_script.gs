//ESP32 to Google Spreadsheet

/* Using spreadsheet API */

function doGet(e) { 
  Logger.log( JSON.stringify(e) );  // view parameters

  var result = 'Ok'; // assume success

  if (e.parameter == undefined) {
    result = 'No Parameters';
  }
  else {
    var id = '1gAdetTpQ3Yrp7kEqNktFEx44TFpGSHBg_sQ4VrrRgcY'; // Spreadsheet ID
    var sheet = SpreadsheetApp.openById(id).getActiveSheet();
    var newRow = sheet.getLastRow() + 1;
    var rowData = [];
    //var waktu = new Date();

    // timezone = "IST+" + new Date().getTimezoneOffset()/60
    //var date = Utilities.formatDate(new Date(), timezone, "yyyy-MM-dd HH:mm"); // "yyyy-MM-dd'T'HH:mm:ss'Z'"
     //var Curr_Time = Utilities.formatDate(new Date(), "Asia/Kolkata", 'HH:mm:ss');
   // rowData[1] = Curr_Time;

    rowData[0] = new Date(); // Timestamp in column A
    var Curr_Time = Utilities.formatDate(new Date(), "Asia/Kolkata", 'HH:mm:ss');
    rowData[1] = Curr_Time;

    for (var param in e.parameter) {
      Logger.log('In for loop, param='+param);
      var value = stripQuotes(e.parameter[param]);
      //Logger.log(param + ':' + e.parameter[param]);
      switch (param) {
      
        case 'voltage': // 
       rowData[2] = value;
        break;
          
        case 'current': // 
       rowData[3] = value;
        break;
            
        case 'power': //
       rowData[4] = value;
        break;
          
        case 'units': // 
       rowData[5] = value;
        break;
         
        default:
          result = "unsupported parameter";
      }
    }
    Logger.log(JSON.stringify(rowData));

    // Write new row below
      var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
      newRange.setValues([rowData]);
  }

  // Return result of operation
  return ContentService.createTextOutput(result);
}

/**
* Remove leading and trailing single or double quotes
*/
function stripQuotes( value ) {
  return value.replace(/^["']|['"]$/g, "");
}
