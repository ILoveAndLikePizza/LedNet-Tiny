/*
Copyright © 2025 ngkon

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

String generateWebpage(byte dmx[], BuiltInLighting led) {
  char color[6];
  sprintf(color, "%02x%02x%02x", (int) (led.r * 255), (int) (led.g * 255), (int) (led.b * 255));

  String output = "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title>LedNet Tiny</title><style>";
  output.concat("body { margin: 0; background: #222; color: #fff; font-family: Balsamiq Sans, sans-serif; }");
  output.concat("h1 { width: 100vw; position: fixed; top: 0; text-align: center; font-size: 4em; }");
  output.concat(".channel input { writing-mode: vertical-rl; direction: rtl; } #channeltbl { display: flex; gap: 5px; margin-top: 15%; }");
  output.concat(".channel { display: flex; flex-direction: column; } #fix div { width: 50%; text-align: center; }");
  output.concat("#fix { display: flex; position: fixed; width: 100vw; justify-content: space-evenly; align-items: center;}");
  output.concat("#fix input { background: inherit; color: inherit; font-family: inherit; font-size: 1.5em; width: 50%;");
  output.concat("border: 1px solid; min-height: 40px; margin: 4px; border-radius: 6px; }");
  output.concat("#fix input[type='button'] { width: calc(50% + 6px); cursor: pointer; } #fix input:hover { background: #ffffff19; }");
  output.concat("h5 { font-style: italic; color: #999; margin: 0; position: fixed; bottom: 6px; left: 6px; }");
  output.concat("</style></head><body><h1>LedNet Tiny</h1><div id=\"channeltbl\">");
  for (int i=1; i<=512; i++) {
    output.concat("<div class=\"channel\"><span>");
    output.concat(i);
    output.concat("</span><input type=\"range\" id=\"dmx-");
    output.concat(i);
    output.concat("\" min=\"0\" max=\"255\" value=\"");
    output.concat(dmx[i]);
    output.concat("\"></div>");
  }
  output.concat("</div><div id=\"fix\"><div><h2>Built-in LED color</h2><input type=\"color\" id=\"led\" value=\"#");
  output.concat(color);
  output.concat("\"></div><div id=\"connect\"><h2>Connect to a network (optional, but easier)</h2><input id=\"ssid\" placeholder=\"SSID...\"><br>");
  output.concat("<input type=\"password\" id=\"pw\" placeholder=\"Password...\"><br><input type=\"button\" value=\"Connect\" onclick=\"connect()\"></div></div>");
  output.concat("<script>const req = (path, body) => fetch(`${location.origin}${path}`");
  output.concat(",{method: 'POST', body, headers: {'Content-Type': 'application/x-www-form-urlencoded'}});");
  output.concat("const dmxs = document.querySelectorAll(`[id^='dmx-']`);");
  output.concat("for (let i=0; i<dmxs.length; i++) dmxs[i].onchange = ");
  output.concat("() => {req('/channel', `c=${i+1}&v=${dmxs[i].value}`); };");
  output.concat("document.getElementById('led').onchange = e => { const r = parseInt(e.target.value.slice(1, 3), 16) / 255;");
  output.concat("const g = parseInt(e.target.value.slice(3, 5), 16) / 255; const b = parseInt(e.target.value.slice(5, 7), 16) / 255;");
  output.concat("req('/light', new URLSearchParams({r, g, b}).toString()); };");
  output.concat("const connect = () => { const ssid = document.getElementById('ssid').value;");
  output.concat("const pw = document.getElementById('pw').value; req('/connect', `ssid=${ssid}&pw=${pw}`);");
  output.concat("alert(`Credentials successfully saved! Reconnect the power to apply!`); }</script><h5>v");
  output.concat(VERSION);
  output.concat(" &nbsp;-&nbsp; &copy; 2025 ngkon. Licensed as free software (GPLv3 or later).</h5></body></html>");

  return output;
};
