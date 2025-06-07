<tr>
  <form method="post" action="<%PostAction%>">
  <td>
    <input type="radio" name="AcceptDeny" value="ACCEPT" <%AcceptCheck%>>Accept<br>
    <input type="radio" name="AcceptDeny" value="DENY" <%DenyCheck%>>Deny
  </td>
  <td>
    <input type="text" name="IPMask" size="15" value="<%IPMask%>" maxlength="25">
  </td>
  <td>
    <nobr>
    <input class=textbox type="Submit" name="Update" Value="Update">
    <input class=button type="Submit" name="Delete" Value="Delete">
    </nobr>
  </td>
  </form>
</tr>
